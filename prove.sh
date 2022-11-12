#!/bin/bash
SCRIPTDIR="$(dirname "${BASH_SOURCE[0]}")"
cd "$SCRIPTDIR"
make test
FP2CONV_LIB="$SCRIPTDIR/libfp2conv.so" python3 -m unittest -v "$@"
FP2CONV_LIB="$SCRIPTDIR/libfp2conv.so" prove -l
