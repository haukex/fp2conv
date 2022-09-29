#!perl
package Data::FP2;
use warnings;
use strict;

=head1 Name

Data::FP2 - FFI bindings for fp2conv.so

=head1 Synopsis

 use Data::FP2 qw/ strtofp2 fp2tostr FP2_FAIL /;
 
 my $fp2 = strtofp2($string);
 die "strtofp2 failed" if $fp2 == FP2_FAIL;
 
 my $str = fp2tostr($fp2);
 die "fp2tostr failed" unless length($str);

=head1 Author, Copyright and License

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

use FindBin;
use File::Basename 'fileparse';
use File::Spec::Functions qw/ catfile updir /;
use Cwd 'abs_path';
use FFI::Platypus;
use Exporter 'import';

use constant { # from #defines in fp2conv.h
	FP2_POS_INF => 0x1FFF,
	FP2_NEG_INF => 0x9FFF,
	FP2_NAN     => 0x9FFE,
	FP2_FAIL    => 0xFFFF,
};

our @EXPORT_OK = qw/ strtofp2 fp2tostr FP2_POS_INF FP2_NEG_INF FP2_NAN FP2_FAIL /;

# dependent on the actual directory layout, you may need to adjust this as needed
our $FP2CONV_SO = abs_path( catfile((fileparse(__FILE__))[1], updir, updir, 'fp2conv.so') )
	or die "Failed to find fp2conv.so at expected location: $!";

my $ffi = FFI::Platypus->new( api => 2, lib => $FP2CONV_SO );
$ffi->attach(                'strtofp2'   => ['string']             => 'uint16_t' );
$ffi->attach( ['fp2tostr' => '_fp2tostr'] => ['uint16_t', 'string'] => 'uint8_t'  );

sub fp2tostr {
	my $fp2 = shift;
	return if $fp2<0 || $fp2>0xFFFF;
	my $str = "\0" x 7;
	return unless _fp2tostr($fp2, $str)>0;
	return unpack 'Z*', $str;
}

1;
