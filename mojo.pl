#!/usr/bin/perl
use 5.10.0;
use Mojolicious::Lite;
use List::Util qw( shuffle );
use List::MoreUtils qw( mesh );

use constant PI                 => 3.141592;
use constant SIGNALS_PER_LINE   => 9000;
use constant FREQUENCY_OF_LA    => 440;
use constant SAMPLING_FREQUENCY => 44100;
use constant SIGNALS            => SAMPLING_FREQUENCY / 4;
use constant VOLUME             => 32768 / 4;

my %SIGNALS;

open my $SERVER, "|./aq.out"
    or die "open: $!";

get q{} => sub {
    my $self = shift;
    $self->render( text => "ok" );
};

post "init" => sub {
    my $self = shift;
    init_signals( );
    $self->render( text => "ok" );
};

post "shuffle" => sub {
    my $self = shift;
    %SIGNALS = do {
        my @keys   = shuffle keys   %SIGNALS;
        my @values = shuffle values %SIGNALS;
        mesh @keys, @values;
    };
    $self->render( text => "ok" );
};

post "stop" => sub {
    my $self = shift;
    my $text = close $SERVER ? "ok" : "Could not close server.[$!]";
    $self->render( text => $text );
};

post "play" => sub {
    my $self = shift;
    my $char = $self->param( "char" );
    my $code = $self->param( "code" );
    my $num  = ord $char || $code
        or return $self->render( text => "fail", status => "400" );

    my @signals = @{ $SIGNALS{ $num } };

    local $| = 1;

    while ( my @numbers = splice @signals, 0, SIGNALS_PER_LINE ) {
        say { $SERVER } join "\t", @numbers;
    }

    $self->render( text => "ok" );
};

get "input" => sub {
    my $self = shift;
    $self->render;
};

get "signals/sin" => sub {
    my $self = shift;
    my @signals;
    my $frequency = get_frequency( );

    for ( my $i = 0; $i < 50; $i++ ) {
        my $signal = int( 10 * sin( 2 * PI * $frequency * $i / 50 ) );
        $signals[ $i ] = [ $i + 1, $signal ];
    }

    $self->render(json => \@signals);
};

get "signals/impulse" => sub {
    my $self = shift;
    my @signals;

    for ( my $i = 0; $i < @signals; $i++ ) {
        $signals[ $i ] = [ $i, $signals[ $i ] ];
    }

    $self->render(json => \@signals);
};

init_signals( );

app->static->root( "public" );
app->start( "datemon" );

sub init_signals { %SIGNALS = map { $_ => [ be_impulse( get_signals( get_frequency( ) ) ) ] } 0 .. 127 }

sub get_frequency { int( FREQUENCY_OF_LA * ( 2 ** ( 1 / ( 1 + int rand 12 ) ) ) ) }

sub get_signals {
    my $frequency = shift;
    my @signals;
    $#signals = SIGNALS - 1;

    for ( my $i = 0; $i < SIGNALS; $i++ ) {
        $signals[ $i ] = sin( 2 * PI * $frequency * $i / SAMPLING_FREQUENCY ) * VOLUME;
    }

    return @signals;
}

sub be_impulse {
    my @signals = @_;
    my $count   = @signals;

    for ( my $i = 0; $i < @signals; $i++ ) {
        my $n    = 50 * $i / $count;
        my $gain = 1.8 * ( 0.9 ** $n ) - 0.8 * ( 0.4 ** $n );
        $signals[ $i ] = $gain * $signals[ $i ];
    }

    return @signals;
}

__DATA__
@@ layouts/layout.html.ep
<!doctype html>
<html lang="ja">
  <head>
    <title>Play with key</title>
    <script src="//code.jquery.com/jquery-1.6.2.min.js"></script>
    <style>
      fieldset { border-style: none }
    </style>
  </head>
  <body>
    <%= content %>
  </body>
</html>

@@ input.html.ep
%layout "layout";
<h1>play</h1>
<form>
  <fieldset>
    <textarea cols="80" rows="24"></textarea>
    <input type="button" value="shuffle" name="shuffle" />
  </fieldset>
</form>
<script>
  $("textarea").keydown(function (e) {
    $.post("http://localhost:3000/play", { code: e.keyCode });
  });
  $("input[type=button][name=shuffle]").click(function () {
    $.post("http://localhost:3000/shuffle");
  });
</script>

