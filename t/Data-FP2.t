#!/usr/bin/env perl
use warnings;
use strict;
use Test::More;
use FindBin;
use File::Spec::Functions qw/ catdir updir /;

=head1 Synopsis

Tests for Data::FP2

=head1 Author, Copyright, and License

Copyright (c) 2022 Hauke Daempfling (haukex@zero-g.net)
at the Leibniz Institute of Freshwater Ecology and Inland Fisheries (IGB),
Berlin, Germany, L<https://www.igb-berlin.de/>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see L<https://www.gnu.org/licenses/>

=cut

diag "This is Perl $] at $^X on $^O";

# parse the C code - this is obviously very much dependent on the format of $testfile
my $testfile = catdir($FindBin::Bin, updir, 'test_fp2conv.c');
note "parsing tests from $testfile";
my $ccode = do { open my $fh, '<', $testfile or die "$testfile: $!"; local $/; <$fh> };

# get NUM_TESTS
my ($numtests) = $ccode=~/^\s*#\s*define\s+NUM_TESTS\s+\(([0-9+* \t]+)\)\s*$/m
	or die "failed to find NUM_TESTS";
# stringy eval is "safe" here because we restricted the characters in the regex
$numtests = eval $numtests or die "failed to parse NUM_TESTS";

# get TEST
my ($teststr) = $ccode=~/\bchar\s*\*\s*TEST\s*\[\s*NUM_TESTS\s*\]\s*=\s*\{(.+?)\}/s
	or die "failed to find TEST";
$teststr =~ s#^\s*//.*(?:\n|\z)##mg; # remove comments
my @tests = map { s/^\s+|\s+$//g; $_ } grep /\S/, split ',', $teststr;

# done parsing out tests
die "parsing failed: NUM_TESTS ($numtests) doesn't match length of TEST (".(0+@tests).")"
	unless @tests==$numtests;
my $num_roundtrip_tests = grep { /^"R/ } @tests;

# now actually run the tests
plan tests => 1 + $num_roundtrip_tests*2 + ($numtests - $num_roundtrip_tests) + 2;

use_ok 'Data::FP2', 'strtofp2', 'fp2tostr', 'FP2_FAIL';
my $loc = $Data::FP2::FP2CONV_SO = $Data::FP2::FP2CONV_SO; # avoid "used only once"
note "module loaded $loc";

for my $test (@tests) {
	my ($type,$fp2,$str) =
		$test =~ /\A\s*"([RPS])((?:\\x[A-Fa-f0-9]{2}){2}) ([^"]*)"\s*\z/
			or die $test;
	$fp2 =~ tr/0-9a-fA-F//cd;
	$fp2 = hex($fp2);
	if ( $type ne 'S' )
		{ is strtofp2($str), $fp2, sprintf("'%s' -> %#04x", $str, $fp2) }
	if ( $type ne 'P' ) {
		if ( $fp2==FP2_FAIL() or !length($str) )
			{ is fp2tostr($fp2), undef, sprintf("%#04x -> fail", $fp2) }
		else
			{ is fp2tostr($fp2), $str, sprintf("%#04x -> '%s'", $fp2, $str) }
	}
}

is fp2tostr(-1), undef, "-1 -> fail";
is fp2tostr(0x10000), undef, "0x10000 -> fail";

