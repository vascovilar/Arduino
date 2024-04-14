#ifndef Psram_h
#define Psram_h

#include "Arduino.h"


class Psram
{
  static const int  _PSRAM_SIZE = 4194304; // ESP32 wrover-B

  protected:
  
    bool    _initPSRAM();
    
    template<typename T> 
    void    _bindPSRAM(T &value, int length)
    {
      if (ESP.getFreePsram() > 0) {
        value = (T) ps_malloc(length);
      }
    }
    
    template<typename T> 
    void    _freePSRAM(T &value) 
    {
      free(value);
    }
};

#endif
