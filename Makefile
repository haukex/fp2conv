
.PHONY : all test ardulibinst clean

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic
ARDU_LIB_PATH = $(HOME)/Arduino/libraries

all: fp2conv.so

fp2conv.so: fp2conv.c fp2conv.h
	$(CC) $(CFLAGS) -shared -o "$@" -fPIC fp2conv.c

test_fp2conv_tests.inc.c: gen_fp2conv_tests.py tests.json
	python3 gen_fp2conv_tests.py

test: fp2conv.so test_fp2conv.c test_fp2conv_tests.inc.c
	$(CC) $(CFLAGS) -Werror -o test_fp2conv test_fp2conv.c fp2conv.so -Wl,-rpath=$(shell pwd) && ./test_fp2conv

ardulibinst:
	@test -d "$(ARDU_LIB_PATH)" || { echo "Please set ARDU_LIB_PATH correctly; $(ARDU_LIB_PATH) does not exist"; exit 1; }
	mkdir -p "$(ARDU_LIB_PATH)/fp2conv"
	cp fp2conv.[ch] "$(ARDU_LIB_PATH)/fp2conv/"

clean:
	rm -f fp2conv.so test_fp2conv

