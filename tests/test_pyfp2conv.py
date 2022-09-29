#!/usr/bin/env python3
"""Unit tests for pyfp2conv.

Author, Copyright, and License
------------------------------
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
import unittest
import json
from pathlib import Path
from pyfp2conv import strtofp2, fp2tostr, FP2_FAIL

class TestFP2Conv(unittest.TestCase):

    def setUp(self):
        with open( Path(__file__).resolve().parent.parent/'tests.json' ) as fh:
            self.tests = [ t for t in json.load(fh) if not isinstance(t, str) ]

    def test_strtopf2(self):
        for t,f,s in ( x for x in self.tests if x[0]!='S' ):
            fp2 = int(f,16)
            if fp2==FP2_FAIL:
                with self.assertRaises(ValueError): strtofp2(s)
            else:
                self.assertEqual( fp2, strtofp2(s) )

    def test_fp2tostr(self):
        for t,f,s in ( x for x in self.tests if x[0]!='P' ):
            fp2 = int(f,16)
            if fp2==FP2_FAIL or len(s)<1:
                with self.assertRaises(ValueError): fp2tostr(fp2)
            else:
                self.assertEqual( s, fp2tostr(fp2) )
        with self.assertRaises(ValueError): fp2tostr(-1)
        with self.assertRaises(ValueError): fp2tostr(0x10000)

if __name__ == '__main__':  # pragma: no cover
    unittest.main()

