/*
 *  Name:     Typewriter
 *  Author:   Bruce E. Hall
 *  Date:     26 Aug 2021
 *  
 *  SUPER-SIMPLE PS/2 KEYBOARD TEST
 *  Using the "EasyKey" library
 *  
 *  Press a key, and the character
 *  is echoed to the serial monitor
 *  
 */

#include <EasyKey.h>                  // https://github.com/bhall66/EasyKey

#define CLOCK_PIN            0        // specify pin for keyboard clock line
#define DATA_PIN             1        // specify pin for keyboard data line
#define BAUDRATE        115200        // serial monitor baud rate

EasyKey kbd;                          // 'kbd' is the keyboard object

void setup()   {     
  kbd.begin(DATA_PIN, CLOCK_PIN);     // initialize keyboard library          
  Serial.begin(BAUDRATE);             // initialize serial monitor
}

void loop() { 
  if (kbd.available()) {              // As soon as a key is pressed:
    char c = kbd.read();              // get the keyboard character
    Serial.print(c);                  // echo it to serial monitor
  }
}
