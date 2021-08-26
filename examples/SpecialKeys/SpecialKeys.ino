/*
 *  Name:     SpecialKeys
 *  Author:   Bruce E. Hall
 *  Date:     26 Aug 2021
 *  
 *  Demonstrates recognition of special keys
 *  like Page-Up, Left-Arrow, Ctrl-C, Alt-A
 *  using the EasyKey library
 *  
 *  Press a key, and the results are
 *  sent to the serial monitor
 *  
 */

#include <EasyKey.h>                  // https://github.com/bhall66/EasyKey

#define CLOCK_PIN            0        // specify pin for keyboard clock line
#define DATA_PIN             1        // specify pin for keyboard data line
#define BAUDRATE        115200        // specify the serial monitor baud rate

EasyKey kbd;                          // 'kbd' is the keyboard object


void setup()   {     
  kbd.begin(DATA_PIN, CLOCK_PIN);     // initialize keyboard library          
  Serial.begin(BAUDRATE);             // initialize serial monitor
}

void loop() { 
  if (kbd.available()) {              // As soon as a character is available,
    char c = kbd.read();              // take it from the buffer.

    // the following switch..case statement includes a sampling of special
    // keys that can be recognized.   See the full listing in EasyKey.h.
    
    switch(c) {
      case PS2_ESC:        Serial.println("Escape"); break;
      case PS2_PAGEUP:     Serial.println("Page Up"); break;
      case PS2_PAGEDOWN:   Serial.println("Page Down"); break;
      case PS2_HOME:       Serial.println("Home"); break;
      case PS2_END:        Serial.println("End"); break;
      case PS2_INSERT:     Serial.println("Insert"); break;
      case PS2_DELETE:     Serial.println("Delete"); break;
      case PS2_LEFTARROW:  Serial.println("Left Arrow"); break;
      case PS2_RIGHTARROW: Serial.println("Right Arrow"); break;
      case PS2_UPARROW:    Serial.println("Up Arrow"); break;
      case PS2_DOWNARROW:  Serial.println("Down Arrow"); break;
      case PS2_CTRL_C:     Serial.println("Ctrl-C"); break;
      case PS2_CTRL_Z:     Serial.println("Ctrl-Z"); break;
      case PS2_F1:         Serial.println("F1 key"); break;
      case PS2_ALT_A:      Serial.println("Alt-A"); break;
      default:             Serial.println(c); break;
    }
  }
}
