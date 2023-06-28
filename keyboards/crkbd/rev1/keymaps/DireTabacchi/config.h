/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

//#define USE_MATRIX_I2C

/* Select hand configuration */

//#define TAPPING_TOGGLE 2
#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

// Mousekeys settings
#define MK_3_SPEED
#define MK_C_OFFSET_0 2
#define MK_C_OFFSET_1 6
#define MK_C_OFFSET_2 16

// WPM settings
#define WPM_SAMPLE_SECONDS 10
#define WPM_SAMPLE_PERIODS 30
//#define QUICK_TAP_TERM 200
//#define TAPPING_TERM 200

// N-key rollover
#define FORCE_NKRO

#define MINUTE_TO_MS 60000

#ifdef OLED_ENABLE
    #define OLED_TIMEOUT 10 * MINUTE_TO_MS
#endif

