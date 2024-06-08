#ifndef Data_h
#define Data_h

#include "Arduino.h"


// *********************************
// Enums required for structures
// *********************************

enum vcolor {
  COLOR_TRANSPARENT = 0xffffff, // transparent color is defined as COLOR_WHITE wich is not often used for backgrounds in a dark skin IHM
  COLOR_RED = 0xff0000,
  COLOR_GREEN = 0x00ff00,
  COLOR_BLUE = 0x0000ff,
  COLOR_CYAN = 0x00ffff,
  COLOR_MAGENTA = 0xff00ff,
  COLOR_YELLOW = 0xffff00,
  COLOR_ORANGE = 0xffa500,
  COLOR_VIOLET = 0xee82ee,
  COLOR_VIOLET_DARK = 0x4032a8,
  COLOR_WHITE = 0xffffff,
  COLOR_WHITE_DARK = 0xeeeeee,
  COLOR_GREY = 0x888888,
  COLOR_GREY_DARK = 0x444444,
  COLOR_GREY_DEEPDARK = 0x111111,
  COLOR_BLACK = 0x000000,
};

enum vstatus {
  GRIS = 0,   // inactif
  VERT = 1,   // confort
  JAUNE = 2,  // perturbé
  ORANGE = 3, // gêne
  ROUGE = 4,  // danger
  VIOLET = 5, // maximum
};

static const byte VPAGE_COUNT = 8;

enum vpage { // TODO vasco *** ca ne devrait pas etre là
  NONE = 0,
  HOME_PAGE = 1,
  CURRENT_EVENT = 2,
  BUFFER_UP = 3,
  BUFFER_DOWN = 4,
  EAR_REALTIME = 5,
  EMF_REALTIME = 6,
  SAT_TABLE = 7,
};

enum vdirection {// TODO vasco *** ca ne devrait pas etre là
  UP = 0,
  DOWN = 1,
  LEFT = 2,
  RIGHT = 3,
};


// *********************************
// Data structures
// *********************************

// sensor structures

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

// mouse structures

struct vpointer {
  int       x;
  int       y;
  bool      focus;
  bool      click;
  bool      left;
  bool      right;
  bool      up;
  bool      down;
};

struct vzone {
  vpage     page;
  float     x1;
  float     y1;
  float     x2;
  float     y2;
};

// imu structures

struct vcoord {
  float     x;
  float     y;
  float     z;
};


class Data
{
  protected:

    vcolor  _convert(vstatus status);

};

#endif
