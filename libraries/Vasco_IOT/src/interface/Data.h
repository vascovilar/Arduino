#ifndef Data_h
#define Data_h

#include "Arduino.h"


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
  COLOR_BLUE = 0x0000FF,
};

enum vstatus {
  GRIS = 0,   // inactif
  VERT = 1,   // confort
  JAUNE = 2,  // perturbé
  ORANGE = 3, // gêne
  ROUGE = 4,  // danger
  VIOLET = 5, // maximum
};

// *********************************
// Data structures
// *********************************

struct vfield {
  String    label;
  String    unit;
  int       tolerance;
  float     value;
  vstatus   status;
  String    text;
};

struct vlegend {
  float     limit;
  vstatus   status;
  String    text;
};

struct vmouse {
  int       x;
  int       y;
  bool      focus;
  bool      click;
  bool      left;
  bool      right;
  bool      up;
  bool      down;
};

struct vcoord {
  float     x;
  float     y;
  float     z;
};

struct vsnap {
  float*    buffer;
  int       length;
  float     maximum;
  int       time;
};

class Data
{
  protected:

    // Tool: needed for precision, map function use integers only
    float _isometric(float value, float maximum, float minimum, int height, int offset)
    {
      return (1 - ((value - minimum) / (float)(maximum - minimum))) * height + offset;
    }
};

#endif
