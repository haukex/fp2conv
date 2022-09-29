#!/usr/bin/env python3
"""Python wrapper for fp2conv C library.

Author, Copyright and License
-----------------------------
Copyright (c) 2022 Hauke Daempfling (haukex@zero-g.net)
at the Leibniz Institute of Freshwater Ecology and Inland Fisheries (IGB),
Berlin, Germany, https://www.igb-berlin.de/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see https://www.gnu.org/licenses/
"""
from pathlib import Path
import ctypes

_clib = ctypes.CDLL(Path(__file__).resolve().parent/"fp2conv.so")

_strtofp2 = _clib.strtofp2
_strtofp2.argtypes = [ctypes.c_char_p]
_strtofp2.restype = ctypes.c_uint16

_fp2tostr = _clib.fp2tostr
_fp2tostr.argtypes = [ctypes.c_uint16, ctypes.c_char_p]
_fp2tostr.restype = ctypes.c_uint8

# from #defines in fp2conv.h
FP2_POS_INF = 0x1FFF
FP2_NEG_INF = 0x9FFF
FP2_NAN     = 0x9FFE
FP2_FAIL    = 0xFFFF

def strtofp2(string :str) -> int:
    """Convert a string to a FP2 value."""
    rv = _strtofp2(string.encode('ASCII'))
    if rv==FP2_FAIL: raise ValueError(f"strtofp2 failed to parse {string!r}")
    return rv

def fp2tostr(fp2 :int) -> str:
    """Convert a FP2 value to a string."""
    if not 0<=fp2<=0xFFFF: raise ValueError(f"fp2 value must be 16-bit unsigned int")
    buf = ctypes.create_string_buffer(7)
    rv = _fp2tostr(fp2, buf)
    if rv<1: raise ValueError(f"fp2tostr failed to convert {fp2:#04x}")
    assert rv==len(buf.value)
    return buf.value.decode('ASCII')

