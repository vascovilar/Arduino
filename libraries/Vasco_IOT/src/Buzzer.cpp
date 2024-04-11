#include "Buzzer.h"

bool Buzzer::init()
{
  if (!_initPWM(_pwmPin, _PWM_CHANNEL)) {
    Serial.println(F("Error initializing buzzer device"));
    return false;
  }
  
  return true;
}

bool Buzzer::wake()
{
  return true;
}

bool Buzzer::sleep()
{ 
  return true;
}

bool Buzzer::check()
{ 
  return false;
}

bool Buzzer::update()
{ 
  return false;
}

void Buzzer::beep()
{
  _note(1, 2300, NOIRE); // resonant frequency
}

void Buzzer::notice()
{
  _note(4, LA, DOUBLE_CROCHE);
}

void Buzzer::warning()
{
  _note(4, LA, CROCHE);
}

void Buzzer::alert()
{
  _note(3, LA, TRIPLE_CROCHE);
  _silence(SOUPIR);
  _note(3, LA, TRIPLE_CROCHE);
}

void Buzzer::open()
{
  _note(1, LA, BLANCHE);
  _note(3, LA, CROCHE);
}

void Buzzer::close()
{
  _note(3, LA, NOIRE);
  _note(1, LA, NOIRE);
}

void Buzzer::modem()
{
  for (int i; i< 40; i++) {
    _note((int) random(4), (int) random(1000000), DOUBLE_CROCHE);
  }
}

void Buzzer::isabel()
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

void Buzzer::_note(byte octave, int milliHertz, int milliSeconds)
{
  float frequencyInHerz = (float) (pow(2, octave) * milliHertz) / 1000.0;

  _tonePWM((int) frequencyInHerz);
  delay(milliSeconds);
  _silence(0);
}

void Buzzer::_silence(int milliSeconds)
{
  _tonePWM(0);
  delay(milliSeconds);
}

