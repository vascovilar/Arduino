#include "BUZ001.h"

bool BUZ001::init()
{
  if (!_initPWM(_pwmPin, _PWM_CHANNEL)) {
    Serial.println(F("Error initializing BUZ001 device"));
    
    return false;
  }
  
  return true;
}

bool BUZ001::wake()
{
  return true;
}

bool BUZ001::sleep()
{ 
  return true;
}

bool BUZ001::check()
{ 
  return false;
}

bool BUZ001::update()
{ 
  return false;
}

void BUZ001::beep()
{
  _note(1, 2300, NOIRE); // resonant frequency
}

void BUZ001::notice()
{
  _note(4, LA, DOUBLE_CROCHE);
}

void BUZ001::warning()
{
  _note(4, LA, CROCHE);
}

void BUZ001::alert()
{
  _note(3, LA, TRIPLE_CROCHE);
  _silence(SOUPIR);
  _note(3, LA, TRIPLE_CROCHE);
}

void BUZ001::open()
{
  _note(1, LA, BLANCHE);
  _note(3, LA, CROCHE);
}

void BUZ001::close()
{
  _note(3, LA, NOIRE);
  _note(1, LA, NOIRE);
}

void BUZ001::modem()
{
  for (int i; i< 40; i++) {
    _note((int) random(4), (int) random(1000000), DOUBLE_CROCHE);
  }
}

void BUZ001::isabel()
{
  int music[44] = {
    2, DO, CROCHE, DEMI_PAUSE,
    2, DO, CROCHE, DEMI_PAUSE,
    2, DO, CROCHE, DEMI_PAUSE,
    2, RE, CROCHE, DEMI_PAUSE,
    2, MI, NOIRE, DEMI_PAUSE,    
    2, RE, NOIRE, DEMI_PAUSE,
    2, DO, NOIRE, DEMI_PAUSE,
    2, MI, NOIRE, DEMI_PAUSE,
    2, RE, NOIRE, DEMI_PAUSE,
    2, RE, NOIRE, DEMI_PAUSE,
    2, DO, BLANCHE, DEMI_PAUSE    
  };
  for (int i = 0; i < 44; i+=4) {
    _note(music[i], music[i+1], music[i+2]); 
    _silence(music[i+3]);  
  }
}

void BUZ001::_note(byte octave, int milliHertz, int milliSeconds)
{
  float frequencyInHerz = (float) (pow(2, octave) * milliHertz) / 1000.0;

  _tonePWM((int) frequencyInHerz);
  delay(milliSeconds);
  _silence(0);
}

void BUZ001::_silence(int milliSeconds)
{
  _tonePWM(0);
  delay(milliSeconds);
}

