#include "../interface/Data.h"

vcolor  Data::_convert(vstatus status)
{
  switch (status) {
    case GRIS:
      return COLOR_GREY_DARK;
    case VERT:
      return COLOR_GREEN;
    case JAUNE:
      return COLOR_YELLOW;
    case ORANGE:
      return COLOR_ORANGE;
    case ROUGE:
      return COLOR_RED;
    case VIOLET:
      return COLOR_VIOLET;
  }

  return COLOR_BLACK;
}