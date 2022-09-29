#!/bin/bash
cd "$(dirname "${BASH_SOURCE[0]}")"
make test
python3 -m unittest -v "$@"
prove -l
