#include "VDeviceSound.h"

void VDeviceSound::begin(int pin)
{
  ledcSetup(0, 2300, 14);
  ledcAttachPin(pin, 0);
}

void VDeviceSound::beep()
{
  _note(1, 2300, NOIRE); // resonant frequency
}

void VDeviceSound::notice()
{
  _note(6, LA, DOUBLE_CROCHE);
}

void VDeviceSound::warning()
{
  _note(4, LA, CROCHE);
}

void VDeviceSound::alert()
{
  _note(3, LA, TRIPLE_CROCHE);
  _silence(SOUPIR);
  _note(3, LA, TRIPLE_CROCHE);
}

void VDeviceSound::open()
{
  _note(1, LA, BLANCHE);
  _note(3, LA, CROCHE);
}

void VDeviceSound::close()
{
  _note(3, LA, NOIRE);
  _note(1, LA, NOIRE);
}

void VDeviceSound::modem()
{
  for (int i; i< 40; i++) {
    _note((int) random(4), (int) random(1000000), DOUBLE_CROCHE);
  }
}

void VDeviceSound::isabel()
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

void VDeviceSound::_note(int octave, int milliHertz, int milliSeconds)
{
  float frequencyInHerz = (float) (pow(2, octave) * milliHertz) / (float) 1000;

  ledcWriteTone(0, (int) frequencyInHerz);
  delay(milliSeconds);
  ledcWriteTone(0, 0);
}

void VDeviceSound::_silence(int milliSeconds)
{
  sleep(milliSeconds);
}

