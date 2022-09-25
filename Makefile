
.PHONY : all test clean

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic

all: fp2conv.so

fp2conv.so: fp2conv.c fp2conv.h
	$(CC) $(CFLAGS) -shared -o fp2conv.so -fPIC fp2conv.c

test: test_fp2conv.c fp2conv.c fp2conv.h
	$(CC) $(CFLAGS) -Werror -o test_fp2conv fp2conv.c test_fp2conv.c && ./test_fp2conv

clean:
	rm -f fp2conv.so test_fp2conv

