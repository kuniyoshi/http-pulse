#!/usr/bin/perl
use 5.10.0;
use utf8;
use strict;
use warnings;
use open qw( :utf8 :std );
use LWP::UserAgent;

use constant COUNT => 30;

my $ua = LWP::UserAgent->new;

my @chars = ( ( "a" .. "z" ), ( "A" .. "Z" ), ( 0 .. 9 ) );

$ua->post( "http://localhost:3000/play", { char => $_ } )
    foreach @chars[ map { int rand @chars } 1 .. COUNT ];


