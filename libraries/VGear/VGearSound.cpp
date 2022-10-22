#include "VGearSound.h"

void VGearSound::begin(int pin)
{
  ledcAttachPin(pin, 0);
}

void VGearSound::beep()
{
  ledcWriteTone(0, 2000);
  delay(10);
  ledcWriteTone(0, 0);
}

void VGearSound::open()
{
  ledcWriteTone(0, 440);
  delay(100);
  ledcWriteTone(0, 1760);
  delay(50);
  ledcWriteTone(0, 0);
}

void VGearSound::close()
{
  ledcWriteTone(0, 1760);
  delay(100);
  ledcWriteTone(0, 440);
  delay(50);
  ledcWriteTone(0, 0);
}
