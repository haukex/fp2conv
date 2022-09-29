
.PHONY : all test clean

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic

all: fp2conv.so

fp2conv.so: fp2conv.c fp2conv.h
	$(CC) $(CFLAGS) -shared -o "$@" -fPIC fp2conv.c

test_fp2conv_tests.inc.c: gen_fp2conv_tests.py tests.json
	python3 gen_fp2conv_tests.py

test: fp2conv.so test_fp2conv.c test_fp2conv_tests.inc.c
	$(CC) $(CFLAGS) -Werror -o test_fp2conv test_fp2conv.c fp2conv.so -Wl,-rpath=$(shell pwd) && ./test_fp2conv

clean:
	rm -f fp2conv.so test_fp2conv

