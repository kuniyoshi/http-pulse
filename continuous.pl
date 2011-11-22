#!/usr/bin/perl
use 5.10.0;
use utf8;
use strict;
use warnings;
use open qw( :utf8 :std );
use Time::HiRes qw( sleep );
use Log::Sigil;

use constant PI                 => 3.141592;
use constant SIGNALS_PER_LINE   => 9000;
use constant FREQUENCY_OF_LA    => 440;
use constant SAMPLING_FREQUENCY => 44100;
use constant SIGNALS            => SAMPLING_FREQUENCY / 4;
use constant VOLUME             => 32768 / 4;

my $log = Log::Sigil->instance;

open my $SERVER, "|./aq.out"
    or die "open: $!";

my $true = 1;
$SIG{INT} = sub {
    $true = 0;
};

while ( $true ) {
    my @signals = be_impulse( get_signals( ) );

    local $| = 1;

    while ( my @numbers = splice @signals, 0, SIGNALS_PER_LINE ) {
        say { $SERVER } join "\t", @numbers;
        sleep 0.3;
    }

}

close $SERVER
    or die "close: $!";

exit;

sub get_signals {
    my @signals;
    $#signals = SIGNALS - 1;

    my $frequency = FREQUENCY_OF_LA * ( 2 ** ( 1 / ( 1 + int rand 12 ) ) );
$log->warn( "frequency: $frequency" );

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

