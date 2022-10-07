fp2conv
=======

"FP2" is a 16-bit data type that is defined to store values
from +/-0.001 to +/-7999 (up to 4 significant digits).

**NOTE:** This data type is not my invention; it is already used on
numerous embedded devices.

This library provides functions to convert these data types to and
from strings, see `fp2conv.h` for details. The data type is defined
as follows:

 | bit 15 (MSB)        |   bits 14 - 13   | bits 12 - 0 (LSB) |
 | :------------------ | :--------------: | ----------------: |
 | sign (0=pos, 1=neg) | decimal position |       significand |

The meanings of bits 14-13 (decimal position) are defined as follows:

 | bit 14 | bit 13 | dec. pos. | value range        |
 | -----: | -----: | --------: | -----------------: |
 |      0 |      0 |   `XXXX.` | +/-800 to +/-7999. |
 |      0 |      1 |   `XXX.X` | +/- 80 to +/-799.9 |
 |      1 |      0 |   `XX.XX` | +/-  8 to +/-79.99 |
 |      1 |      1 |   `X.XXX` |      0 to +/-7.999 |

Note that identical values can be represented in different ways. This
library uses the above value range to decide the storage format, thus
for example the integer 1 is stored as `0x63E8`, even though it could
also be stored as `0x4064`, `0x200A`, or `0x0001`. As one exception,
zero (0) is stored as `0x0000`. Note the conversion from FP2 to string
can handle all of the aforementioned values.

Though theoretically 13 bits can store values from 0 to 8191,
this data type defines the maximum value at 7999; in addition,
the following special values are defined:

 |  Value | FP2 bytes |
 | ------ | --------- |
 |   +Inf |  `0x1FFF` |
 |   -Inf |  `0x9FFF` |
 |    NaN |  `0x9FFE` |
 | Fail\* |  `0xFFFF` |

\* This is not part of the original specification, but as far as I can
tell this value should never occur otherwise, so this code uses it to
indicate that the conversion could not be performed.

Compilation
-----------

Using the provided `Makefile`, you can simply run `make` to build
`libfp2conv.so`, and `make test` to build and run `test_fp2conv.c`.
The library and headers can be installed via `sudo make install`.
Tested on Linux with gcc 7.5.0.

Arduino
-------

The `fp2conv.c` / `fp2conv.h` files were tested to be working on an
Arduino MKR series board with a SAMD21 processor
using the Arduino IDE 2.0.0.

The source files can be installed to your local Arduino library path
with `make ardulibinst`, assuming the default environment on Linux.

Python
------

Simple Python bindings are provided in `pyfp2conv.py` with tests in `tests/`.
Tested on Python 3.10 on Linux.

Perl
----

Simple Perl FFI bindings are provided in `lib/Data/FP2.pm` with tests in `t/`.
The CPAN module `FFI::Platypus` is required.
Tested on Perl 5.34 and 5.28 on Linux.

Note: If `FFI::Platypus` fails to build due to a missing `xlocale.h`,
a workaround is: `cd /usr/include && sudo ln -si locale.h xlocale.h`

Tests
-----

All tests can be run via the included shell script `prove.sh`.


Author, Copyright, and License
------------------------------

Copyright (c) 2022 Hauke Daempfling <haukex@zero-g.net>
at the Leibniz Institute of Freshwater Ecology and Inland Fisheries (IGB),
Berlin, Germany, <https://www.igb-berlin.de/>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
