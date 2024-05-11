#ifndef Device_h
#define Device_h

#include "Arduino.h"
#include "Chipset.h"


class Device : public Chipset
{
  public:

    Device(vchipset code) : Chipset(code, false) { }
};

#endif
