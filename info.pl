#!/usr/bin/perl
use 5.10.0;
use utf8;
use strict;
use warnings;
use open qw( :utf8 :std );
use List::Util qw( min max );
use Data::Dumper qw( Dumper );
use Audio::Wav;

my $audio = Audio::Wav->read( shift );
warn Dumper $audio->details;



__END__
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

__END__
while ( my @chanels = $audio->read ) {
    warn Dumper \@chanels;
}

