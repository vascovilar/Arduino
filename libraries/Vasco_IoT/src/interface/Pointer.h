#ifndef Pointer_h
#define Pointer_h

#include "Arduino.h"
#include "../interface/Data.h"


class Pointer
{
    public:

      virtual vpointer  get() = 0;
      virtual void      set(vpointer pointer) = 0;
      virtual void      reset() = 0;
};

#endif
