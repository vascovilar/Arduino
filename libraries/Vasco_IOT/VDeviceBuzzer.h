/*
 * Piezo disk driver 
 * Implementation:
 *
 *   #include <VDeviceBuzzer.h>
 *
 *   VDeviceBuzzer sound;
 *
 *   void setup() {
 *     sound.begin(2);
 *   }
 *   void loop() {
 *     sound.beep(440, 10);
 *     delay(1000);
 *   }
 */

#ifndef VDeviceBuzzer_h
#define VDeviceBuzzer_h

#include "Arduino.h"
#include "VDevice.h"

class VDeviceBuzzer : public VDevice, public VPwmPin
{
  static const byte _PWM_CHANNEL = 0;

  public:

    VDeviceBuzzer(byte pin) : VDevice(CUSTOM_BUZZER) { _pwmPin = pin; }
    
    bool init();
    bool wake();
    bool sleep();
    
    void beep();
    void notice();
    void warning();
    void alert();
    void open();
    void close();
    void modem();
    void isabel();
    void test() { _tonePWM(2300); }

  private:

    byte  _pwmPin;

    void  _note(byte octave, int milliHertz, int milliSeconds);
    void  _silence(int milliSeconds);

    enum note_frequency {
      DO = 261625,
      DOd = 277182,
      RE = 293664,
      REd = 311126,
      MI = 329627,
      FA = 349228,
      FAd = 369994,
      SOL = 391995,
      SOLd = 415304,
      LA = 440000,
      LAd = 466163,
      SI = 493883
    };

    enum duration_time {
      CARREE = 667,
      RONDE = 333,
      BLANCHE = 166,
      NOIRE = 83,
      CROCHE = 41,
      DOUBLE_CROCHE = 20,
      TRIPLE_CROCHE = 10,
      LIEE = 0
    };

    enum silence_time {
      BATON_DE_PAUSE = 667, 
      PAUSE = 333, 
      DEMI_PAUSE = 166,
      SOUPIR = 83,
      DEMI_SOUPIR = 41,
      QUART_DE_SOUPIR = 20,
      HUITIEME_DE_SOUPIR = 10
    };

};

#endif
