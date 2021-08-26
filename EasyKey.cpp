/*
  EasyKey.cpp - PS2 Keyboard library (originally PS2Keyboard.cpp)
  
  Provenance ---------------------------------------------
  Original Author: Christian Weichel <info@32leaves.net>
  Rewritten June 2010: Paul Stoffregen <paul@pjrc.com>
  Updated by L. Abraham Smith, <n3bah@microcompdesign.com> 
  Updated by Cuningan <cuninganreset@gmail.com>
  Rewritten Aug 2021:  Bruce Hall <bhall66@gmail.com>
  --------------------------------------------------------
  
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
 
  
  Modified by Bruce Hall <bhall66@gmail.com> on 15 Aug 2021
  with the following functional changes:
  1.  Removed default interrupt pin assigments
  2.  Removed non-US keyboards
  3.  Removed non-ASCII output
  4.  Added ctrl-key and alt-key support
  5.  Added a method to clear the keystroke buffer
  6.  Added a method to get raw scancode data

  for more information you can read the original wiki in arduino.cc
  at http://www.arduino.cc/playground/Main/PS2Keyboard
  or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html

  
  Version 2.4 (March 2013)
  - Support Teensy 3.0, Arduino Due, Arduino Leonardo & other boards
  - French keyboard layout, David Chochoi, tchoyyfr at yahoo dot fr

  Version 2.3 (October 2011)
  - Minor bugs fixed

  Version 2.2 (August 2011)
  - Support non-US keyboards - thanks to Rainer Bruch for a German keyboard :)

  Version 2.1 (May 2011)
  - timeout to recover from misaligned input
  - compatibility with Arduino "new-extension" branch
  - TODO: send function, proposed by Scott Penrose, scooterda at me dot com

  Version 2.0 (June 2010)
  - Buffering added, many scan codes can be captured without data loss
    if your sketch is busy doing other work
  - Shift keys supported, completely rewritten scan code to ascii
  - Slow linear search replaced with fast indexed table lookups
  - Support for Teensy, Arduino Mega, and Sanguino added
  
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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include "EasyKey.h"

#define BUFFER_SIZE 45
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail;
static uint8_t DataPin;
static uint8_t CharBuffer=0;
static const keymap_t *keymap=NULL;

// The ISR for the external interrupt
void ps2interrupt(void)
{
	static uint8_t bitcount=0;
	static uint8_t incoming=0;
	static uint32_t prev_ms=0;
	uint32_t now_ms;
	uint8_t n, val;

	val = digitalRead(DataPin);
	now_ms = millis();
	if (now_ms - prev_ms > 250) {
		bitcount = 0;
		incoming = 0;
	}
	prev_ms = now_ms;
	n = bitcount - 1;
	if (n <= 7) {
		incoming |= (val << n);
	}
	bitcount++;
	if (bitcount == 11) {
		uint8_t i = head + 1;
		if (i >= BUFFER_SIZE) i = 0;
		if (i != tail) {
			buffer[i] = incoming;
			head = i;
		}
		bitcount = 0;
		incoming = 0;
	}
}

// The following routine returns the next available byte from the scan // code buffer. A zero is returned if there are no codes in buffer.

static uint8_t get_scancode()
{
	uint8_t c, i;

	i = tail;
	if (i == head) return 0;
	i++;
	if (i >= BUFFER_SIZE) i = 0;
	c = buffer[i];
	tail = i;
	return c;
}

// http://www.quadibloc.com/comp/scan.htm
// http://www.computer-engineering.org/ps2keyboard/scancodes2.html

// This array provide a simple key map, to turn scan codes into ASCII
// output.  If a non-US keyboard is used, these may need to be modified
// for the desired output.
//
// Not all key-combinations have been mapped.  For example,
// only <CTRL-A> through <CTRL-Z> and <ALT-A> through <Alt-Z> generate
// unique characters.  Characters 201-254 are unused, so with the keymap
// below you can add any open CTRL or ALT combination to a character in
// this range.


const PROGMEM keymap_t myKeymap = {
    // standard, non-shifted keys
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,
	0, 0, 0, 0, 0, 'q', '1', 0,
	0, 0, 'z', 's', 'a', 'w', '2', 0,
	0, 'c', 'x', 'd', 'e', '4', '3', 0,
	0, ' ', 'v', 'f', 't', 'r', '5', 0,
	0, 'n', 'b', 'h', 'g', 'y', '6', 0,
	0, 0, 'm', 'j', 'u', '7', '8', 0,
	0, ',', 'k', 'i', 'o', '0', '9', 0,
	0, '.', '/', 'l', ';', 'p', '-', 0,
	0, 0, '\'', 0, '[', '=', 0, 0,
	PS2_CAPSLOCK, 0, PS2_ENTER, ']', 0, '\\', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, PS2_NUMLOCK,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
	
    // with shift
	{0, PS2_SHIFT_F9, 0, PS2_SHIFT_F5, PS2_SHIFT_F3, PS2_SHIFT_F1, PS2_SHIFT_F2, PS2_SHIFT_F12,
	0, PS2_SHIFT_F10, PS2_SHIFT_F8, PS2_SHIFT_F6, PS2_SHIFT_F4, PS2_TAB, '~', 0,
	0, 0, 0, 0, 0, 'Q', '!', 0,
	0, 0, 'Z', 'S', 'A', 'W', '@', 0,
	0, 'C', 'X', 'D', 'E', '$', '#', 0,
	0, ' ', 'V', 'F', 'T', 'R', '%', 0,
	0, 'N', 'B', 'H', 'G', 'Y', '^', 0,
	0, 0, 'M', 'J', 'U', '&', '*', 0,
	0, '<', 'K', 'I', 'O', ')', '(', 0,
	0, '>', '?', 'L', ':', 'P', '_', 0,
	0, 0, '"', 0, '{', '+', 0, 0,
	PS2_CAPSLOCK, 0, PS2_ENTER, '}', 0, '|', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, PS2_NUMLOCK,
	PS2_SHIFT_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_SHIFT_F7 
	},

	// with CTRL
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,
	0, 0, 0, 0, 0, PS2_CTRL_Q, '1', 0,
	0, 0, PS2_CTRL_Z, PS2_CTRL_S, PS2_CTRL_A, PS2_CTRL_W, '2', 0,
	0, PS2_CTRL_C, PS2_CTRL_X, PS2_CTRL_D, PS2_CTRL_E,'4', '3', 0,
	0, ' ', PS2_CTRL_V, PS2_CTRL_F, PS2_CTRL_T, PS2_CTRL_R, '5', 0,
	0, PS2_CTRL_N, PS2_CTRL_B, PS2_CTRL_H, PS2_CTRL_G, PS2_CTRL_Y, '6', 0,
	0, 0, PS2_CTRL_M, PS2_CTRL_J, PS2_CTRL_U, '7', '8', 0,
	0, ',', PS2_CTRL_K, PS2_CTRL_I, PS2_CTRL_O, '0', '9', 0,
	0, '.', '/', PS2_CTRL_L, ';', PS2_CTRL_P, '-', 0,
	0, 0, '\'', 0, '[', '=', 0, 0,
	PS2_CAPSLOCK, 0, PS2_ENTER, ']', 0, '\\', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, PS2_NUMLOCK,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 
	},
	
	// with ALT
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,
	0, 0, 0, 0, 0, PS2_ALT_Q, '1', 0,
	0, 0, PS2_ALT_Z, PS2_ALT_S, PS2_ALT_A, PS2_ALT_W, '2', 0,
	0, PS2_ALT_C, PS2_ALT_X, PS2_ALT_D, PS2_ALT_E,'4', '3', 0,
	0, ' ', PS2_ALT_V, PS2_ALT_F, PS2_ALT_T, PS2_ALT_R, '5', 0,
	0, PS2_ALT_N, PS2_ALT_B, PS2_ALT_H, PS2_ALT_G, PS2_ALT_Y, '6', 0,
	0, 0, PS2_ALT_M, PS2_ALT_J, PS2_ALT_U, '7', '8', 0,
	0, ',', PS2_ALT_K, PS2_ALT_I, PS2_ALT_O, '0', '9', 0,
	0, '.', '/', PS2_ALT_L, ';', PS2_ALT_P, '-', 0,
	0, 0, '\'', 0, '[', '=', 0, 0,
	PS2_CAPSLOCK, 0, PS2_ENTER, ']', 0, '\\', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, PS2_NUMLOCK,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 
	}
};

// The state byte contains the following bits
#define BREAK     0x01
#define EXTENDED  0x02
#define SHIFT     0x04
#define CTRL      0x08
#define ALT       0x10
#define WIN       0x20    // defined, but not currently used


// The following routine converts a scancode to its ASCII equivalent, using
// bits from the 'state' variable to determine whether it should convert a
// normal key, a shifted key, a control key, or an ALT key

char convertToASCII(byte scanCode, byte state) {
	char c = 0;
	if (state & EXTENDED) {                             // convert EXTENDED code
		switch (scanCode) {
		  case 0x1F: c = PS2_LEFTWIN;     break;
		  case 0x27: c = PS2_RIGHTWIN;    break;
		  case 0x2F: c = PS2_MENU;        break;
		  case 0x70: c = PS2_INSERT;      break;
		  case 0x6C: c = PS2_HOME;        break;
		  case 0x7D: c = PS2_PAGEUP;      break;
		  case 0x71: c = PS2_DELETE;      break;
		  case 0x69: c = PS2_END;         break;
		  case 0x7A: c = PS2_PAGEDOWN;    break;
		  case 0x75: c = PS2_UPARROW;     break;
		  case 0x6B: c = PS2_LEFTARROW;   break;
		  case 0x72: c = PS2_DOWNARROW;   break;
		  case 0x74: c = PS2_RIGHTARROW;  break;
		  case 0x4A: c = '/';             break;
		  case 0x5A: c = PS2_ENTER;       break;
		  default: break;
		}
	} else if ((state & CTRL)) {                        // convert CTRL code
		c = pgm_read_byte(
		keymap->ctrl + scanCode);
    } else if ((state & ALT)) {                         // convert ALT code
		c = pgm_read_byte(
		keymap->alt + scanCode);
	} else if (state & SHIFT) {                         // convert SHIFT code
		c = pgm_read_byte(
		keymap->shift + scanCode);
	} else {                                            // convert normal code
		c = pgm_read_byte(
		keymap->noshift + scanCode);
	}
	return c;
}

// The following routine parses bytes from the scan code buffer and outputs
// the ASCII code correponsing to the next character found. 
// It sets flags in the 'state' variable to keep track of special codes
// (extended codes, break codes, and shift/ctrl/alt-key modifiers).  
// A zero is returned if there are no codes in buffer or the code is not found.

static char getChar(void)
{
	static uint8_t state=0;                        // bit flags for current state
	uint8_t s;                                     // current scancode

	while (1) {
		s = get_scancode();                        // get the next code
		if (!s) return 0;                          // no codes left in buffer
		if (s == 0xF0) state |= BREAK;             // set BREAK state
		else if (s == 0xE0) state |= EXTENDED;     // set EXTENDED state
		else {                                     // must be a regular scan code
		
			if (state & BREAK) {                   // handle a BREAK code ----
				if ((s == 0x12)||(s == 0x59))      // breaking the SHIFT key?
					state &= ~SHIFT;               // ..clear SHIFT
				else if (s == 0x14)                // breaking the CTRL key?
					state &= ~CTRL;                // ..clear CTRL
				else if (s == 0x11)                // breaking the ALT key?
					state &= ~ALT;                 // ..clear ALT
				state &= ~(BREAK | EXTENDED);      // clear the BREAK
				
			} else if ((s == 0x12)||(s == 0x59)){  // handle a MAKE code ----
				state |= SHIFT;                    // if char is SHIFT, set SHIFT bit
			} else if (s == 0x14) {                // if char is a CTRL key
				state |= CTRL;                     // ..set CTRL bit
				state &= ~EXTENDED;                // map R-CTRL to CTRL
			} else if (s == 0x11) {                // if char is an ALT key
				state |= ALT;                      // ..set ALT bit
				state &= ~EXTENDED;                // map R-ALT to ALT
			} else {                               // if not Shift/Ctrl/Alt,
			    return(convertToASCII(s,state));   // get the ASCII code
				state &= ~(BREAK | EXTENDED);
			}
		}
	}
}

bool EasyKey::available() {
	if (CharBuffer) return true;
	CharBuffer = getChar();
	if (CharBuffer) return true;
	return false;
}

int EasyKey::read() {
	uint8_t result = CharBuffer;
	if (result) CharBuffer = 0;
	else result = getChar();
	if (!result) return -1;
	return result;
}

// clears the keyboard buffer, erasing all unread keystrokes
void EasyKey::clear() {
	head = 0;
	tail = 0;
	CharBuffer = 0;
}

// returns next available scancode, 0 if none
int EasyKey::getScancode() {
	return get_scancode();
}

EasyKey::EasyKey() {
  // nothing to do here, begin() does it all
}

void EasyKey::begin(uint8_t data_pin, uint8_t irq_pin) {

  // initialize the pins
#ifdef INPUT_PULLUP
  pinMode(irq_pin, INPUT_PULLUP);
  pinMode(data_pin, INPUT_PULLUP);
#else
  pinMode(irq_pin, INPUT);
  digitalWrite(irq_pin, HIGH);
  pinMode(data_pin, INPUT);
  digitalWrite(data_pin, HIGH);
#endif

  head = 0;
  tail = 0;
  attachInterrupt(irq_pin, ps2interrupt, FALLING);
  DataPin = data_pin;
  keymap = &myKeymap;
}


