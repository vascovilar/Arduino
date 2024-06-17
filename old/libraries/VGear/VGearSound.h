/*
 * Make sound with piezzo
 * Implementation:
 *
 *   #include <VGearSound.h>
 *
 *   VGearSound sound; 
 *
 *   void setup() {
 *     sound.begin(12);
 *   }
 *   void loop() {
 *     sound.beep();
 *   }
 */

#ifndef VGearSound_h
#define VGearSound_h

#include "Arduino.h"

class VGearSound
{
  public:

    void begin(int pin);
    void beep();
    void note(int frequency);
    void open();
    void close();

  private:

};

#endif
