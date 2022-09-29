#!/usr/bin/env python3
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

