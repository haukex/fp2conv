
.PHONY : all test install ardulibinst clean

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic
DEST_LIB ?= /usr/local/lib
DEST_INC ?= /usr/local/include
ARDU_LIB_PATH ?= $(HOME)/Arduino/libraries

all: libfp2conv.so

libfp2conv.so: fp2conv.c fp2conv.h
	$(CC) $(CFLAGS) -shared -o "$@" -fPIC fp2conv.c

test_fp2conv_tests.inc.c: gen_fp2conv_tests.py tests.json
	python3 gen_fp2conv_tests.py

test: libfp2conv.so test_fp2conv.c test_fp2conv_tests.inc.c
	$(CC) $(CFLAGS) -Werror -o test_fp2conv test_fp2conv.c -Wl,-rpath=$(shell pwd) -L. -lfp2conv && ./test_fp2conv

install: libfp2conv.so fp2conv.h
	cp libfp2conv.so "$(DEST_LIB)/"
	cp fp2conv.h "$(DEST_INC)/"
	chmod 644 "$(DEST_LIB)/libfp2conv.so" "$(DEST_INC)/fp2conv.h"
	@echo "NOTE you may need to do 'sudo ldconfig' after first install (assuming $(DEST_LIB) is in /etc/ld.so.conf)"

ardulibinst:
	@test -d "$(ARDU_LIB_PATH)" || { echo "Please set ARDU_LIB_PATH correctly; '$(ARDU_LIB_PATH)' does not exist"; exit 1; }
	mkdir -p "$(ARDU_LIB_PATH)/fp2conv"
	cp fp2conv.[ch] "$(ARDU_LIB_PATH)/fp2conv/"

clean:
	rm -f libfp2conv.so test_fp2conv

