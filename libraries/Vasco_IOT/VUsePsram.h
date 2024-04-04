#ifndef VUsePsram_h
#define VUsePsram_h

#include "Arduino.h"
#include "VUse.h"


class VUsePsram
{
  static const int _PSRAM_SIZE = 4194304; // ESP32 wrover-B

  protected:

    bool _initPSRAM();
    
    template<typename T> 
    void _bindPSRAM(T &value, int length)
    {
      if (ESP.getFreePsram() > 0) {
        // return by reference
        value = (T) ps_malloc(length);
      }
    }

    template<typename T> 
    void _freePSRAM(T &value) 
    {
      free(value);
    }
};

#endif
