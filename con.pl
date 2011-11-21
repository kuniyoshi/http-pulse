#!/usr/bin/perl
use 5.10.0;
use utf8;
use strict;
use warnings;
use open qw( :utf8 :std );
use Time::HiRes qw( sleep );

open my $ECHO, "|./con.out"
    or die $!;

my $true = 1;
$SIG{INT} = sub { undef $true };

while ( $true ) {
    say { $ECHO } join "\t", map { -500 + int rand 1_000 } 1 .. 500;
    sleep 0.5;
}

close $ECHO
    or die $!;

