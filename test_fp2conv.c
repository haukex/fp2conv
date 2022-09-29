
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

#include "test_fp2conv_tests.inc.c"

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

