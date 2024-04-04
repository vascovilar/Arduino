#ifndef VUse_h
#define VUse_h

#include "Arduino.h"


#define VEMORY_SIZE  16
enum vmemory_index {
  ROM_BYTE_1 = 0,
  ROM_BYTE_2 = 1,
  ROM_BYTE_3 = 2,
  ROM_BYTE_4 = 3,
  ROM_BYTE_5 = 4,
  ROM_BYTE_6 = 5,
  ROM_BYTE_7 = 6,
  ROM_BYTE_8 = 7,
  ROM_FLOAT_1 = 8,
  ROM_FLOAT_2 = 12,
};

struct vuser_credential {
  const char* user;
  const char* password;
};


class VUse
{

};

#endif
