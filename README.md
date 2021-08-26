# EasyKey
 A PS/2 Keyboard Library

## Introduction
 
This Arduino library provides an easy-to-use PS/2 keyboard interface.

## Basic Usage

First, include EasyKey in your sketch:

```
#include "EasyKey.h"    // https://github.com/bhall66/EasyKey
```

Next, instantiate an EasyKey object:

```
EasyKey kbd;                           
```

In setup(), call begin() with keyboard clock and data pins, making sure that the clock pin is an interrupt pin.

```
void setup()   {     
  kbd.begin(DATA_PIN, CLOCK_PIN);     // initialize keyboard library          
}
```

Finally, call kbd.available() to check for keyboard input, and kbd.read() to get the next available character:

```
void loop() { 
  if (kbd.available()) {              // As soon as a key is pressed:
    char c = kbd.read();              // get the keyboard character
    Serial.print(c);                  // and echo it to serial monitor
  }
}
```

## Provided Examples

1. Typewriter- Sends keyboard characters to the serial monitor.
2. Scancodes - Sends keyboard scancodes to the serial monitor.
3. SpecialKeys - Recongnizes special keys like Page-Up, Esc, F1, Ctrl-C, etc.


## Methods

1. `begin(int datapin, int clockpin)` - initializes the library.  Call this method from your setup() routine, naming the pins connected to the keyboard data and clock lines.  The clock pin must be an interrupt pin.

2. `available()` - returns true if there is a character in the keyboard buffer waiting to be read.

3. `read()` - returns the next character in the keyboard buffer.

4. `getScancode()` - returns the next available scancode byte in the keyboard buffer.  

5. `clear()` - erases the keyboard buffer.

See comments in EasyKey.cpp for documentation of each method.  EasyKey defines many shift-key, ctrl-key, and alt-key combinations for your use.  See EasyKey.h for a list of the defined key constants.

## Credits

The nidus of this library has been around a long time and may be found in many flavors.  The original code was written by Christian Weichel and called PS2Keyboard.  See the [Arduio Playground page](https://playground.arduino.cc/Main/PS2Keyboard) for more information. 


