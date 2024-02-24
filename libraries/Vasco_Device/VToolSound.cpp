#include "VToolSound.h"

void VToolSound::begin(int pin)
{
  ledcAttachPin(pin, 0);
}

void VToolSound::getBeep()
{
  _note(1, LA, NOIRE);
}

void VToolSound::getNotice()
{
  _note(6, LA, DOUBLE_CROCHE);
}

void VToolSound::getWarning()
{
  _note(4, LA, CROCHE);
}

void VToolSound::getAlert()
{
  _note(3, LA, TRIPLE_CROCHE);
  _silence(SOUPIR);
  _note(3, LA, TRIPLE_CROCHE);
}

void VToolSound::getOpen()
{
  _note(1, LA, BLANCHE);
  _note(3, LA, CROCHE);
}

void VToolSound::getClose()
{
  _note(3, LA, NOIRE);
  _note(1, LA, NOIRE);
}

void VToolSound::getModem()
{
  for (int i; i< 40; i++) {
    _note((int) random(4), (int) random(1000000), DOUBLE_CROCHE);
  }
}

void VToolSound::getIsabel()
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
    _note(music[i], music[i+1], music[i+2]); _silence(music[i+3]);  
  }
}

void VToolSound::_note(int octave, int frequency, int duration)
{
  float frequencyInHerz = (float) (pow(2, octave) * frequency) / (float) 1000;

  ledcWriteTone(0, (int) frequencyInHerz);
  delay(duration);
  ledcWriteTone(0, 0);
}

void VToolSound::_silence(int milliSeconds)
{
  _note(1, 0, milliSeconds);
}

