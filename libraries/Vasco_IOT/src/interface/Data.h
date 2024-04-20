#ifndef Data_h
#define Data_h

#include "Arduino.h"

static const int  DELAY_TO_PUSH_TO_HISTORY = 1000; // 180000 = 3 min means 240 mesures = 12h

static const byte VCOLOR_COUNT = 9;

enum vcolor {
  COLOR_BLACK = 0x000000,
  COLOR_WHITE = 0xFFFFFF,
  COLOR_GREY = 0x999999,
  COLOR_GREY_DARK = 0x444444,
  COLOR_GREEN = 0x4EB400,
  COLOR_YELLOW = 0xF7E400,
  COLOR_ORANGE = 0xF85900,
  COLOR_RED = 0xFF0000,
  COLOR_VIOLET = 0x6B49C8,
  COLOR_TURQUOISE = 0x018387,
};

struct vpointer {
  int     x;
  int     y;
  bool    focus;
  bool    click;
  bool    left;
  bool    right;
  bool    up;
  bool    down;
};

struct vcoord {
  float   x;
  float   y;
  float   z;
};


class Data
{
// TODO vasco add array structure value, history, array data
// TODO vasco add graphic structures text, lines, circles, arrow, big ..
// TODO vasco match champs et devices a placer dans un DataSensors
};

#endif
