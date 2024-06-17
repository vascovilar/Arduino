#ifndef Mouse_h
#define Mouse_h

#include "Arduino.h"
#include "../interface/Data.h"
#include "../interface/Pointer.h"
#include "../interface/Screen.h"
#include "../bin/iconMonoTransparent.h"


class Mouse : public Data
{
  public:

    Mouse(Pointer &obj1, Screen &obj2) : _pointer(obj1), _screen(obj2) {}; // bind objects

    void      add(vzone zone);
    vpage     hover();
    vpage     click();
    void      draw();
    void      clear();

  private:

    Pointer   &_pointer;
    Screen    &_screen;
    vzone     _zone[VPAGE_COUNT];
    int       _index = 0;
    vpointer  _lastPointer;
};

#endif
