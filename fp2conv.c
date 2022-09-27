
/** ***** fp2conv.c *****
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
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <assert.h>

uint16_t strtofp2(const char *str) {
	if ( strcasecmp(str,  "NaN") == 0 ) return FP2_NAN;
	if ( strcasecmp(str,  "Inf") == 0 ) return FP2_POS_INF;
	if ( strcasecmp(str, "-Inf") == 0 ) return FP2_NEG_INF;
	size_t len = strlen(str);
	if ( len<1 || len>6 ) return FP2_FAIL; // input too short or long
	uint8_t neg = 0;
	int intpart = -1;
	int fraclen = -1;
	char sigs[5] = {0,0,0,0,0}; // significand
	size_t sptr = 0;
	for ( size_t i=0; i<len; i++ ) {
		if ( i==0 && str[i]=='-' ) neg = 1;
		else if ( i<len-1 && str[i]=='.' ) {
			// dot at beginning, or duplicate dot
			if ( sptr<1 || intpart>-1 ) return FP2_FAIL;
			intpart = atoi(sigs);
			fraclen = 0;
		}
		else if ( str[i]>='0' && str[i]<='9' ) {
			if (sptr>=4) return FP2_FAIL; // input too long
			sigs[sptr++] = str[i];
			if ( fraclen>-1 ) fraclen++;
		}
		else return FP2_FAIL; // invalid character
	}
	if ( sptr<1 ) return FP2_FAIL; // no digits
	int sig = atoi(sigs);
	if ( sig>7999 || sig<0 ) return FP2_FAIL; // out of range
	assert( intpart < sig );
	if ( intpart<0 ) { // no dot was seen
		assert(fraclen<0);
		intpart=sig;
		fraclen=0;
	} else assert(fraclen>-1 && fraclen<4);
	// Note the following code has a lot of potential to be optimized.
	int dot = 0x0000; // this is correct for 800<=intpart<8000
	if ( sig==0 ) { /*dot = 0x0000;*/ } // special case for 0
	else if ( intpart<8 ) { // 0<=intpart<8
		dot = 0x6000;
		if (intpart==0) {
			assert(fraclen>0); // must be true because sig!=0 here
			if (fraclen==1) sig*=100;
			else if (fraclen==2) sig*=10;
		}
		else {
			if (sig<8) sig*=1000;
			else if (sig<80) sig*=100;
			else if (sig<800) sig*=10;
		}
	} 
	else if ( intpart<80  ) { // 8<=intpart<80
		dot = 0x4000;
		if (sig<80) sig*=100;
		else if (sig<800) sig*=10;
	}
	else if ( intpart<800 ) { // 80<=intpart<800
		dot = 0x2000;
		if (sig<800) sig*=10;
	}
	
	return ( neg ? 0x8000 : 0x0000 ) | dot | ( sig & 0x1FFF );
}

uint8_t fp2tostr(uint16_t fp2, char *outbuf) {
	if ( !outbuf ) return 0; // null pointer
	if ( fp2==FP2_NAN     ) { strncpy(outbuf,  "NaN", 7); return 3; }
	if ( fp2==FP2_POS_INF ) { strncpy(outbuf,  "Inf", 7); return 3; }
	if ( fp2==FP2_NEG_INF ) { strncpy(outbuf, "-Inf", 7); return 4; }
	uint8_t dot = (fp2>>13)&0x3;
	int val = fp2&0x1FFF;
	if (val>7999) return 0;
	// Note the following code has a lot of potential to be optimized.
	char *neg = (fp2&0x8000) ? "-" : "";
	int len;
	switch (dot) {
		case 0: len = snprintf(outbuf, 7, "%s%d",   neg, val); break;
		// Floating-point accuracy should always be better than FP2,
		// which is why it should be fine to use floating-point here:
		case 1: len = snprintf(outbuf, 7, "%s%.1f", neg, val/10.0f); break;
		case 2: len = snprintf(outbuf, 7, "%s%.2f", neg, val/100.0f); break;
		case 3: len = snprintf(outbuf, 7, "%s%.3f", neg, val/1000.0f); break;
	}
	if ( len<1 || len>6 ) return 0; // snprintf failed
	if ( dot==0 ) return len;
	// At this point we know there will be a decimal point, so
	// trim off trailing zeros (and dot, if applicable).
	while (len>0) {
		if (outbuf[len-1]=='0') { outbuf[--len]='\0'; }
		else if (outbuf[len-1]=='.') { outbuf[--len]='\0'; return len; }
		else { return len; }
	}
	assert(0); return 0; // unreachable
}

