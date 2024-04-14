/*
 * Simple custom piezo disk driver 
 *
 * Implementation:
 *
 *   #include <BUZ001.h>
 *
 *   BUZ001 sound;
 *
 *   void setup() {
 *     sound.begin(2);
 *   }
 *   void loop() {
 *     sound.beep(440, 10);
 *     delay(1000);
 *   }
 */

#ifndef BUZ001_h
#define BUZ001_h

#include "Arduino.h" 
#include "interface/Device.h"
#include "plugin/Pins.h"


class BUZ001 : public Device, public PwmPin
{
  static const byte _PWM_CHANNEL = 1;

  public:

    BUZ001(byte pin) : Device(BUZZER) { _pwmPin = pin; }
    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update();
    // sounds
    void    beep();
    void    notice();
    void    warning();
    void    alert();
    void    open();
    void    close();
    void    modem();
    void    isabel();

  private:
  
    byte    _pwmPin;
    void    _note(byte octave, int milliHertz, int milliSeconds);
    void    _silence(int milliSeconds);

    enum vnote_frequency {
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
      SI = 493883,
    };

    enum vduration_time {
      CARREE = 667,
      RONDE = 333,
      BLANCHE = 166,
      NOIRE = 83,
      CROCHE = 41,
      DOUBLE_CROCHE = 20,
      TRIPLE_CROCHE = 10,
      LIEE = 0,
    };

    enum vsilence_time {
      BATON_DE_PAUSE = 667, 
      PAUSE = 333, 
      DEMI_PAUSE = 166,
      SOUPIR = 83,
      DEMI_SOUPIR = 41,
      QUART_DE_SOUPIR = 20,
      HUITIEME_DE_SOUPIR = 10,
    };
};

#endif
