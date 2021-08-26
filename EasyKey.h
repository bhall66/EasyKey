/*
  EasyKey.h - PS2 Keyboard library (originally PS2Keyboard.h)
  
  Provenance ---------------------------------------------
  Original Author: Christian Weichel <info@32leaves.net>
  Rewritten June 2010: Paul Stoffregen <paul@pjrc.com>
  Updated by L. Abraham Smith, <n3bah@microcompdesign.com> 
  Updated by Cuningan <cuninganreset@gmail.com>
  Rewritten Aug 2021:  Bruce Hall <bhall66@gmail.com>
  --------------------------------------------------------
  
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
*/

#ifndef EasyKey_h
#define EasyKey_h

#include "Arduino.h"         // for attachInterrupt, FALLING

// Every call to read() returns a single byte for each
// keystroke.  The following defines configure what byte will be returned
// for each "special" key.  To ignore a key, use zero.

#define PS2_CTRL_A          1
#define PS2_CTRL_B          2
#define PS2_CTRL_C          3
#define PS2_CTRL_D          4
#define PS2_CTRL_E          5
#define PS2_CTRL_F          6
#define PS2_CTRL_G          7
#define PS2_CTRL_H          8
#define PS2_CTRL_I          9
#define PS2_CTRL_J          10
#define PS2_CTRL_K          11
#define PS2_CTRL_L          12
#define PS2_CTRL_M          13
#define PS2_CTRL_N          14
#define PS2_CTRL_O          15
#define PS2_CTRL_P          16
#define PS2_CTRL_Q          17
#define PS2_CTRL_R          18
#define PS2_CTRL_S          19
#define PS2_CTRL_T          20
#define PS2_CTRL_U          21
#define PS2_CTRL_V          22
#define PS2_CTRL_W          23
#define PS2_CTRL_X          24
#define PS2_CTRL_Y          25
#define PS2_CTRL_Z          26

#define PS2_TAB				9     // same as ctrl-i
#define PS2_ENTER			13    // same as ctrl-m
#define PS2_ESC				27    

#define PS2_BACKSPACE		127
#define PS2_DELETE			128
#define PS2_INSERT			129
#define PS2_HOME			130
#define PS2_END				131
#define PS2_PAGEUP			132
#define PS2_PAGEDOWN		133
#define PS2_UPARROW			134
#define PS2_LEFTARROW		135
#define PS2_DOWNARROW		136
#define PS2_RIGHTARROW		137
#define PS2_SCROLL			138
#define PS2_LEFTWIN         139
#define PS2_RIGHTWIN        140
#define PS2_MENU            141
#define PS2_CAPSLOCK        142
#define PS2_NUMLOCK         143

#define PS2_F1				150
#define PS2_F2				151
#define PS2_F3				152
#define PS2_F4				153
#define PS2_F5				154
#define PS2_F6				155
#define PS2_F7				156
#define PS2_F8				157
#define PS2_F9				158
#define PS2_F10				159
#define PS2_F11			    160
#define PS2_F12				161

#define PS2_SHIFT_F1        162
#define PS2_SHIFT_F2        163
#define PS2_SHIFT_F3        164
#define PS2_SHIFT_F4        165
#define PS2_SHIFT_F5        166
#define PS2_SHIFT_F6        167
#define PS2_SHIFT_F7        168
#define PS2_SHIFT_F8        169
#define PS2_SHIFT_F9        170
#define PS2_SHIFT_F10       171
#define PS2_SHIFT_F11       172
#define PS2_SHIFT_F12       173

#define PS2_ALT_A           175
#define PS2_ALT_B           176
#define PS2_ALT_C           177
#define PS2_ALT_D           178
#define PS2_ALT_E           179
#define PS2_ALT_F           180
#define PS2_ALT_G           181
#define PS2_ALT_H           182
#define PS2_ALT_I           183
#define PS2_ALT_J           184
#define PS2_ALT_K           185
#define PS2_ALT_L           186
#define PS2_ALT_M           187
#define PS2_ALT_N           188
#define PS2_ALT_O           189
#define PS2_ALT_P           190
#define PS2_ALT_Q           191
#define PS2_ALT_R           192
#define PS2_ALT_S           193
#define PS2_ALT_T           194
#define PS2_ALT_U           195
#define PS2_ALT_V           196
#define PS2_ALT_W           197
#define PS2_ALT_X           198
#define PS2_ALT_Y           199
#define PS2_ALT_Z           200



#define PS2_KEYMAP_SIZE 136

typedef struct {
	uint8_t noshift[PS2_KEYMAP_SIZE];
	uint8_t shift[PS2_KEYMAP_SIZE];
	uint8_t ctrl[PS2_KEYMAP_SIZE];
	uint8_t alt[PS2_KEYMAP_SIZE];
} keymap_t;



/**
 * Purpose: Provides easy access to PS2 keyboards
 */
 
class EasyKey {
  public:
  	/**
  	 * This constructor does basically nothing. Please call the begin(int,int)
  	 * method before using any other method of this class.
  	 */
    EasyKey();
    
	
    /**
     * Starts the keyboard "service" by registering the external interrupt.
     * setting the pin modes correctly and driving those needed to high.
     * The best place to call this method is in the setup routine.
     */
    static void begin(uint8_t dataPin, uint8_t irq_pin);
    
	
    /**
     * Returns true if there is a char to be read, false if not.
     */
    static bool available();
    
	
    /**
     * Returns the char last read from the keyboard.
     * If there is no char availble, -1 is returned.
     */
    static int read();
	
	
	/**
     * Clears the keyboard buffer, erasing all unread keystrokes
     */
	static void clear();
	
	
    /**  
	 * Returns the next available byte from the scan code buffer. 
	 * A zero is returned if there are no codes in buffer.
	 * Note: Do not mix calls to read() and getScancode();
	 */
    static int getScancode();
};

#endif
