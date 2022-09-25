
/** ***** test_fp2conv.c *****
 *
 * Copyright (c) 2022 Hauke Daempfling (haukex@zero-g.net)
 * at the Leibniz Institute of Freshwater Ecology and Inland Fisheries (IGB),
 * Berlin, Germany, https://www.igb-berlin.de/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see https://www.gnu.org/licenses/
 */

#include "fp2conv.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#define UNUSED(x) (void)(x)

#define NUM_TESTS ( 8 + 11*2 + 6*2 + 6*2 + 8*2 + 9 + 8+3 + 14 + 18 )
char *TEST[NUM_TESTS] = {
	// test string format:
	// [0]: "P"=strtofp2 only, "S"=fp2tostr only, "R"=both
	// [1-2]: the FP2 value
	// [3]: a space character
	// [4-]: the string
	
	// basic tests (4 digits)
	"R\x04\xD2 1234",  "R\x84\xD2 -1234",
	"R\x24\xD2 123.4", "R\xA4\xD2 -123.4",
	"R\x44\xD2 12.34", "R\xC4\xD2 -12.34",
	"R\x64\xD2 1.234", "R\xE4\xD2 -1.234",
	
	// out of range
	"R\xFF\xFF 10000",  "R\xFF\xFF 9999",   "R\xFF\xFF 8192",   "R\xFF\xFF 8191",
	"R\xFF\xFF 8000",   "R\xFF\xFF 800.0",  "R\xFF\xFF 800.1",
	
	"R\xFF\xFF -10000", "R\xFF\xFF -9999",  "R\xFF\xFF -8192",  "R\xFF\xFF -8191",
	"R\xFF\xFF -8000",  "R\xFF\xFF -800.0", "R\xFF\xFF -800.1",
	
	// range +/-800 to +/-7999 (dot=00)
	"R\x1F\x3F 7999",   "R\x1F\x3E 7998",
	"R\x03\x21 801",    "R\x03\x20 800",
	
	"R\x9F\x3F -7999",  "R\x9F\x3E -7998",
	"R\x83\x21 -801",   "R\x83\x20 -800",
	
	// range +/-80 to +/-799.9 (dot=01)
	"R\x3F\x3F 799.9",  "R\x3F\x36 799",
	"R\x23\xE9 100.1",  "R\x23\xE8 100",
	"R\x23\x21 80.1",   "R\x23\x20 80",
	
	"R\xBF\x3F -799.9", "R\xBF\x36 -799",
	"R\xA3\xE9 -100.1", "R\xA3\xE8 -100",
	"R\xA3\x21 -80.1",  "R\xA3\x20 -80",
	
	// range +/-8 to +/-79.99 (dot=10)
	"R\x5F\x3F 79.99",  "R\x5E\xDC 79",
	"R\x43\xE9 10.01",  "R\x43\xE8 10",
	"R\x43\x21 8.01",   "R\x43\x20 8",
	
	"R\xDF\x3F -79.99", "R\xDE\xDC -79",
	"R\xC3\xE9 -10.01", "R\xC3\xE8 -10",
	"R\xC3\x21 -8.01",  "R\xC3\x20 -8",
	
	// range 0 to +/-7.999 (dot=11)
	"R\x7F\x3F 7.999",  "R\x7B\x58 7",
	"R\x63\xE9 1.001",  "R\x63\xE8 1",
	"R\x60\x64 0.1",    "R\x60\x0A 0.01",
	"R\x60\x01 0.001",  "R\x00\x00 0",
	
	"R\xFF\x3F -7.999", "R\xFB\x58 -7",
	"R\xE3\xE9 -1.001", "R\xE3\xE8 -1",
	"R\xE0\x64 -0.1",   "R\xE0\x0A -0.01",
	"R\xE0\x01 -0.001", "R\x80\x00 -0",
	
	// special strings
	"R\x1F\xFF Inf", "R\x9F\xFF -Inf", "R\x9F\xFE NaN",
	"P\x1F\xFF INF", "P\x9F\xFF -INF", "P\x9F\xFE NAN",
	"P\x1F\xFF inf", "P\x9F\xFF -inf", "P\x9F\xFE nan",
	
	// stringification tests
	"S\x63\xE8 1", "S\x40\x64 1", "S\x20\x0A 1", "S\x00\x01 1",
	"S\x00\x00 0", "S\x20\x00 0", "S\x40\x00 0", "S\x60\x00 0",
	// invalid FP2 values
	"S\xFF\xFF ", "S\xFF\xFE ", "S\x1F\x40 ",
	
	// parsing tests
	"P\x23\xE8 100",  "P\x23\xE8 100.0",
	"P\x43\xE8 10",   "P\x43\xE8 10.0", "P\x43\xE8 10.00",
	"P\x63\xE8 1",    "P\x63\xE8 1.0",  "P\x63\xE8 1.00",  "P\x63\xE8 1.000",
	"P\x60\x64 0.1",  "P\x60\x64 0.10", "P\x60\x64 0.100",
	"P\x60\x0A 0.01", "P\x60\x0A 0.010",
	
	// invalid inputs for parsing
	"P\xFF\xFF ",    "P\xFF\xFF abcd", "P\xFF\xFF 12345",
	"P\xFF\xFF  12", "P\xFF\xFF 12 ",  "P\xFF\xFF  1 ",
	"P\xFF\xFF --1", "P\xFF\xFF 1-",   "P\xFF\xFF 1x",
	"P\xFF\xFF .5",  "P\xFF\xFF 1..2", "P\xFF\xFF -123456.7",
	"P\xFF\xFF 12.", "P\xFF\xFF -.3",  "P\xFF\xFF 0.0001",
	"P\xFF\xFF +12", "P\xFF\xFF +Inf", "P\xFF\xFF -NaN",
};

int main(int argc, char** argv) {
	UNUSED(argc); UNUSED(argv);
	size_t fails = 0;
	// round-trip tests
	for ( size_t i=0; i<NUM_TESTS; i++ ) {
		// decode test
		if ( ( TEST[i][0]!='R' && TEST[i][0]!='P' && TEST[i][0]!='S' )
		     || TEST[i][3]!=' ' )
			{ printf("FAIL: Bad test spec!\n"); return 2; }
		uint16_t exp_fp2 = (TEST[i][1]<<8) | (TEST[i][2]&0xFF);
		char *str = TEST[i]+4;
		size_t len = strlen(str);
		printf("str=%9s(%ld), exp_fp2=0x%04X", str, len, exp_fp2);
		if ( TEST[i][0]!='S' ) { // test strtofp2
			uint16_t got_fp2 = strtofp2(str);
			printf(" strtofp2=> got_fp2=0x%04X", got_fp2);
			if ( got_fp2==exp_fp2 ) printf(" PASS");
			else { printf(" FAIL"); fails++; }
		}
		if ( TEST[i][0]!='P' ) { // test fp2tostr
			char buf[7] = {0,0,0,0,0,0,0};
			uint8_t rv = fp2tostr(exp_fp2, buf);
			printf(" fp2tostr=> buf=%6s(%d)", buf, rv);
			if ( exp_fp2==FP2_FAIL ) {
				if ( rv==0 ) printf(" PASS");
				else { printf(" FAIL"); fails++; }
			}
			else {
				if ( rv==len && strcasecmp(str,buf)==0 )
					printf(" PASS");
				else { printf(" FAIL"); fails++; }
			}
		}
		printf("\n");
	}
	if ( fails>0 ) printf("%ld FAILURES\n", fails);
	else printf("ALL PASS\n");
	return fails ? 1 : 0;
}

