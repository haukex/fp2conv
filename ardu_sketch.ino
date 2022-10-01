/**
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

/* **WARNING:** Don't compile this in the fp2conv project directory, as
 * the Arduino tools may pick up and compile the other C files as well.
 */

#include "fp2conv.h"

/* Tests with the Arduino IDE 2.0.0 show that including the functions
 * causes the program memory requirements to increase as follows:
 * strtofp2: 19740 bytes
 * fp2tostr: 21188 bytes
 *     both: 22076 bytes
 * while RAM usage increases by 368 bytes in all three cases.
 * The increases are likely due in large part to the library functions used.
 */

void setup() {
  Serial.begin(19200);
  Serial.setTimeout(60000);  // defaults to 1000ms
}

const int BUFSZ = 32; // must be >=7 for fp2tostr
char buf[BUFSZ];

void loop() {
  memset(buf, 0, BUFSZ);
  size_t insz = Serial.readBytesUntil('\n', buf, BUFSZ-1);
  if (insz>0) {
    uint16_t fp2 = strtofp2(buf);
    Serial.println(fp2, HEX);
    if ( fp2!=FP2_FAIL && fp2tostr(fp2, buf)>0 ) {
      Serial.println(buf);
    }
  }
}
