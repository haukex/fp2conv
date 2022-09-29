#!/usr/bin/env python3
from pyfp2conv import fp2tostr
from collections import defaultdict

# I was curious how many values in the FP2 data type are duplicated or unused
# (e.g. '1' can be stored as 0001, 200A, 4064, and 63E8), this script checks

VERBOSE = 0

vals = defaultdict(list)
for i in range(0x10000):
    try: s = fp2tostr(i)
    except ValueError: s = None
    vals[s].append(i)

dupevals = 0
for k,v in sorted( vals.items(), key=lambda x: len(x[1]) ):
    assert len(v)>=1
    if len(v)!=1:
        dupevals += len(v)-1
        if VERBOSE:
            print(f"{k!r}: {', '.join(map(lambda x: '%04X' % x,v))} => {dupevals}")

print(f"of {0x10000} possible values, {dupevals} or {dupevals/0x10000:.1f}%, are duplicated (or unused)")

