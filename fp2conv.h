#ifndef FP2CONV_H
#define FP2CONV_H

/** ***** fp2conv.h *****
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

#include <stdint.h>
#include <stdlib.h>

#define FP2_POS_INF (0x1FFF)
#define FP2_NEG_INF (0x9FFF)
#define FP2_NAN     (0x9FFE)
#define FP2_FAIL    (0xFFFF)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/** Convert a string to a FP2 value.
 * 
 * The FP2 format is described in the README in this repository.
 *
 * `str` is the null-terminated string to convert. The maximum string
 * length is 6 characters (max. of four significant digits, decimal
 * point, and optional minus sign). The decimal point must have digits
 * on either side (i.e. `"123.0"` or `"123"` instead of `"123."`, and
 * `"0.123"` instead of `".123"`). Next to decimal numbers, the strings
 * `"NaN"`, `"Inf"`, and `"-Inf"` are accepted case-insensitively.
 * 
 * If this function returns `FP2_FAIL`, the conversion could not be
 * performed due to an invalid input format.
 */
uint16_t strtofp2(const char *str);

/** Convert a FP2 value to a string.
 * 
 * The FP2 format is described in the README in this repository.
 * 
 * `fp2` should be a valid FP2 value.
 * 
 * `outbuf` *must* be a pointer to a 7-byte buffer for the output,
 * which will be null-terminated iff the conversion is successful.
 *
 * Returns the number of characters written to `outbuf`, excluding the
 * terminating null byte. If this function returns 0, `outbuf` must be
 * discarded as it may not be null-terminated or contain other junk.
 */
uint8_t fp2tostr(uint16_t fp2, char *outbuf);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // #ifndef FP2CONV_H
