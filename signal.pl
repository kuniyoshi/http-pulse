#!/usr/bin/perl
use 5.10.0;
use utf8;
use strict;
use warnings;
use open qw( :utf8 :std );
use Readonly qw( Readonly );
use List::Util qw( min max );
use Time::StopWatchWithMessage;
use Data::Dumper qw( Dumper );
use Audio::Wav;

my $audio = Audio::Wav->read( shift );

my @channels = $audio->read;
print $channels[0];

while ( my @channels = $audio->read ) {
    print "\t", $channels[0];
}

print "\n";


__END__
my $watch = Time::StopWatchWithMessage->new;

$watch->start( "init" );
warn "### init";

my $count;
my $audio = Audio::Wav->read( shift );
my @signals;
my $length = 0;

my %param = %{ $audio->details };

$#signals = $param{data_length} * 8 / $param{bits_sample} / $param{channels};

$watch->stop->start( "reading" );
warn "--- read";

while ( my @channels = $audio->read ) {
    $signals[ $length++ ] = $channels[0];
#    print $sig
}

$watch->stop->start( "printing" );
warn "--- print";

#say join "\t", @signals;

#for (my $i = 0; $i < $#signals; $i++) {
#    warn "not defined [$i]" unless defined $signals[ $i ];
#}

$watch->stop->warn;

__END__
while ( my @chanels = $audio->read ) {
    print $chanels[0], "\n";

#    last
#        if $count++ >= $LIMIT;
}

__END__
my $audio = Audio::Wav->read( shift );
my( $min, $max ) = ( 0, 0 );

while ( my @chanels = $audio->read ) {
    $min = min( $min, $chanels[0] );
    $max = max( $max, $chanels[0] );
}

say "min: $min";
say "max: $max";

__END__
while ( my @chanels = $audio->read ) {
    print $chanels[0], "\n";
}

