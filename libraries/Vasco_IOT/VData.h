#ifndef VData_h
#define VData_h

#include "Arduino.h"

#define VCOLOR_COUNT 9 
enum vcolor_code {
  COLOR_BLACK = 0x000000,
  COLOR_WHITE = 0xFFFFFF,
  COLOR_GREY = 0x999999,
  COLOR_GREY_DARK = 0x444444,
  COLOR_GREEN = 0x4EB400,
  COLOR_YELLOW = 0xF7E400,
  COLOR_ORANGE = 0xF85900,
  COLOR_RED = 0xFF0000,
  COLOR_VIOLET = 0x6B49C8,  
};

// TODO vasco add array structure value, history, array data
// TODO vasco add graphic structures text, lines, circles, arrow, big .. 

class VData
{
  public:

    // TODO vasco match champs et devices a placer dans un VDataSensors
    // TODO vasco formater de données graphe à appliquer par héritage aux presenters
    // TODO vasco: structure d'output du VDataBuffer (presenter / formater) à partager avec les classes utilisant une sortie web ou écran

    // TODO vasco ? add stats (average min max delta trend...) instead of stats in buffer object
     
};

#endif
