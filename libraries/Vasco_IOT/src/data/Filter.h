#ifndef DataFilter_h
#define DataFilter_h

#include "Arduino.h"
#include "../interface/Data.h"


class Filter : public Data
{
  static const byte _BUFFER_MAX_SIZE = 100;

  protected:

    // needed for precision, map function is only integers
    float   _isometric(float value, float maximum, float minimum, int height, int offset);
    // iterators to smooth graphs
    float   _smooth(float value, int factor);
    float   _maximum(float value, int factor);
    float   _inertia(float value, int factor);

  private:

    float   _smoothBuffer[_BUFFER_MAX_SIZE];
    int     _smoothIndex = 0;
    float   _smoothTotal = 0;
    float   _maxBuffer[_BUFFER_MAX_SIZE];
    int     _maxIndex = 0;
    float   _inertiaValue = 0;
};

#endif
