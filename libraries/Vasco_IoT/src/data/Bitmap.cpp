#include "Bitmap.h"


vzone Bitmap::_drawPage(String titleLeft, String titleRight, int forcedWidth)
{
  _screen.rect(VBITMAP_COL_PAGE, 10, VSCREEN_WIDTH + 20, VSCREEN_HEIGHT + 20, COLOR_BLUE, VBITMAP_RADIUS, false); // + 20 means out of screen voluntary
  _drawPill(VBITMAP_COL_MARGIN, 0, titleLeft, titleRight, forcedWidth);

  return (vzone){NONE, 0, 0, VSCREEN_WIDTH, VSCREEN_HEIGHT};
}

vzone Bitmap::_drawPill(float x, float y, String contentLeft, String contentRight, int forcedWidth)
{
  int pillLeftWidth = VBITMAP_RADIUS + _screen.width(contentLeft, SIZE_TITLE) + VBITMAP_PADDING_X;
  int pillRightWidth = VBITMAP_PADDING_X + (forcedWidth == 0 ? _screen.width(contentRight, SIZE_TITLE): forcedWidth) + VBITMAP_RADIUS;
  int pillWidth = forcedWidth == 0 ? pillLeftWidth + pillRightWidth: forcedWidth;
  int pillHeight = _screen.height(contentLeft, SIZE_TITLE) + 2 * VBITMAP_PADDING_Y;
  int pillMiddlePadding = x + pillLeftWidth;

  _screen.rect(x, y, pillLeftWidth + VBITMAP_RADIUS, pillHeight, COLOR_BLUE, VBITMAP_RADIUS); // draw blue left pill
  _screen.rect(pillMiddlePadding, y, pillRightWidth, pillHeight, COLOR_BLACK); // remove right pill
  _screen.rect(pillMiddlePadding - VBITMAP_RADIUS, y, pillRightWidth + VBITMAP_RADIUS, pillHeight, COLOR_BLUE, VBITMAP_RADIUS, false); // draw right not filled
  _screen.text(x + VBITMAP_RADIUS, y + VBITMAP_PADDING_Y, contentLeft, SIZE_TITLE, COLOR_CYAN, COLOR_BLUE); // add left text
  _screen.text(pillMiddlePadding + VBITMAP_PADDING_X, y + VBITMAP_PADDING_Y, contentRight, SIZE_TITLE, COLOR_BLUE, COLOR_BLACK); // add right text + modify handleHistoryUpdate

  return (vzone){NONE, x, y, x + pillWidth, y + pillHeight};
}

vzone Bitmap::_drawButtonMini(float x, float y, String content, vpage target, bool invertColor)
{
  int buttonWidth = _screen.width(content, SIZE_SMALL) + 2 * VBITMAP_PADDING_X;
  int buttonHeight = _screen.height(content, SIZE_SMALL) + 2 * VBITMAP_PADDING_Y - 1;
  int paddingToCenter = buttonWidth / 2 - _screen.width(content, SIZE_SMALL) / 2 + 1;

  if (invertColor) {
    _screen.rect(x, y, buttonWidth, buttonHeight, COLOR_CYAN, 6, false);
    _screen.text(x + paddingToCenter, y + VBITMAP_PADDING_Y, content, SIZE_SMALL, COLOR_CYAN, COLOR_BLACK);
  } else {
    _screen.rect(x, y, buttonWidth, buttonHeight, COLOR_CYAN, 6);
    _screen.text(x + paddingToCenter, y + VBITMAP_PADDING_Y, content, SIZE_SMALL, COLOR_BLACK);
  }

  return (vzone){target, x, y, x + buttonWidth, y + buttonHeight};
}

vzone Bitmap::_drawButtonArrow(float x, float y, vdirection direction, vpage target)
{
  int buttonWidth = 15;
  int buttonHeight = 16;
  int xo = x;
  int yo = y;
  // passing coordinates from arrowhead to standart upper-left
  switch (direction) {
    case UP:
      xo += buttonWidth / 2;
      break;
    case DOWN:
      xo += buttonWidth / 2;
      yo += buttonHeight;
      break;
    case LEFT:
      yo += buttonHeight / 2;
      break;
    case RIGHT:
      xo += buttonWidth;
      yo += buttonHeight / 2;
      break;
  }
  _screen.arrow(xo, yo, buttonWidth, buttonHeight, COLOR_CYAN, direction);

  return (vzone){target, x, y, x + buttonWidth, y + buttonHeight};
}

vzone Bitmap::_drawButtonBack(float x, vpage target)
{
  int buttonBackWidth = 60;
  int alignToBottom = VSCREEN_HEIGHT - 1;

  _screen.line(x, alignToBottom, x + buttonBackWidth, alignToBottom, COLOR_CYAN);

  return (vzone){target, x, alignToBottom, x + buttonBackWidth, alignToBottom};
}

vzone Bitmap::_drawBullet(float x, float y, vcolor color)
{
  int bulletWidth = _screen.width(" ", SIZE_TITLE, true);
  int bulletHeight = _screen.height(" ", SIZE_TITLE);

  _screen.rect(x, y, bulletWidth, bulletHeight, color, VBITMAP_RADIUS / 2);

  return (vzone){NONE, x, y, x + bulletWidth, y + bulletHeight};
}

vzone Bitmap::_drawAnalogBar(float x, float y, float value, float minimum, float maximum, int tolerance)
{
  int bulletWidth = _screen.width(" ", SIZE_TITLE, true);
  int bulletHeight = _screen.height(" ", SIZE_TITLE);
  float valuePercent = 100 * value / (maximum - minimum);

  _drawBullet(x + (bulletWidth + 1) * 0, y, valuePercent > (tolerance * 1) ? COLOR_GREEN: COLOR_BLACK); // + 1 is padding between bullets
  _drawBullet(x + (bulletWidth + 1) * 1, y, valuePercent > (tolerance * 2) ? COLOR_YELLOW: COLOR_BLACK);
  _drawBullet(x + (bulletWidth + 1) * 2, y, valuePercent > (tolerance * 3) ? COLOR_ORANGE: COLOR_BLACK);
  _drawBullet(x + (bulletWidth + 1) * 3, y, valuePercent > (tolerance * 4) ? COLOR_RED: COLOR_BLACK);

  return (vzone){NONE, x, y, x + 5 * (bulletWidth + 1), y + bulletHeight};
}

vzone Bitmap::_drawIcon(float x, float y, const unsigned char* icon, int width, int height, vcolor color, vcolor bgColor)
{
  _screen.bitmap(x, y, icon, width, height, color, bgColor);

  return (vzone){NONE, x, y, x + width, y + height};
}

vzone Bitmap::_drawTableHeader(float x, float y, String* titles, int length)
{
  for (int i = 0; i < length; i++) {
    _screen.text(x, y + VBITMAP_CELLSPACING * i, titles[i], SIZE_TEXT, COLOR_GREY);
    _screen.line(x, 10 + y + VBITMAP_CELLSPACING * i, VSCREEN_WIDTH, 10 + y + VBITMAP_CELLSPACING * i, COLOR_GREY_DEEPDARK);
  }
  // clear last line
  _screen.line(x, 10 + y + VBITMAP_CELLSPACING * (length - 1), VSCREEN_WIDTH, 10 + y + VBITMAP_CELLSPACING * (length - 1), COLOR_BLACK);

  return (vzone){NONE, x, y, VSCREEN_WIDTH, y + 12 * length};
}

vzone Bitmap::_drawTableData(float x, float y, String* values, int length)
{
  for (int i = 0; i < length; i++) {
    _screen.text(x + 90, y + VBITMAP_CELLSPACING * i, String(values[i]), SIZE_TEXT, COLOR_WHITE_DARK, COLOR_BLACK);
  }

  return (vzone){NONE, 100, y, VSCREEN_WIDTH, y + 12 * length};
}

const unsigned char* Bitmap::_getIcon(vsensor code)
{
  switch (code) {
    case TEMPERATURE:
      return VICON32_temperature;
    case PRESSURE:
      return VICON32_pressure;
    case HUMIDITY:
      return VICON32_humidity;
    case AIR_QUALITY:
      return VICON32_quality;
    case GAS_PERCENTAGE:
      return VICON32_particles;
    case EAR_LEVEL:
      return VICON32_vibes;
    case EMF_LEVEL:
      return VICON32_emf;
    case GAUSS_LEVEL:
      return VICON32_magnetic;
    case VISIBLE:
      return VICON32_visible;
    case UV_INDEX:
      return VICON32_uv;
    case ALTITUDE:
      return VICON32_altitude;
    case MOVEMENT:
      return VICON32_movement;
    case MEMORY_USED:
      return VICON32_memory;
    case RUN_CYCLES:
      return VICON32_cycles;
    default:
      return VICON32_stars;
  }
}

void Bitmap::iconTestPage()
{
  // arrows
  _screen.arrow(80, 10, 10, 10, COLOR_GREY, UP);
  _screen.arrow(80, 10, 10, 10, COLOR_GREY, DOWN);
  _screen.arrow(80, 10, 10, 10, COLOR_GREY, LEFT);
  _screen.arrow(80, 10, 10, 10, COLOR_GREY, RIGHT);


  // 24
  _drawIcon(0, 0, VICON12_pointer, 12, 16, COLOR_WHITE);
  _drawIcon(15, 0, VICON24_wifi, 24, 24, COLOR_BLUE);
  _drawIcon(42, 0, VICON24_bluetooth, 24, 24, COLOR_BLUE);

  // 32 line 1
  _drawIcon(0, 40, VICON32_stars, 32, 32, COLOR_GREY);
  _drawIcon(32, 40, VICON32_temperature, 32, 32, COLOR_GREY);
  _drawIcon(64, 40, VICON32_pressure, 32, 32, COLOR_GREY);
  _drawIcon(96, 40, VICON32_humidity, 32, 32, COLOR_GREY);
  _drawIcon(128, 40, VICON32_quality, 32, 32, COLOR_GREY);
  _drawIcon(160, 40, VICON32_particles, 32, 32, COLOR_GREY);
  _drawIcon(192, 40, VICON32_vibes, 32, 32, COLOR_GREY);

  // 32 line 2
  _drawIcon(0, 72, VICON32_emf, 32, 32, COLOR_GREY);
  _drawIcon(32, 72, VICON32_visible, 32, 32, COLOR_GREY);
  _drawIcon(64, 72, VICON32_uv, 32, 32, COLOR_GREY);
  _drawIcon(96, 72, VICON32_altitude, 32, 32, COLOR_GREY);
  _drawIcon(128, 72, VICON32_movement, 32, 32, COLOR_GREY);
  _drawIcon(160, 72, VICON32_memory, 32, 32, COLOR_GREY);
  _drawIcon(192, 72, VICON32_cycles, 32, 32, COLOR_GREY);

  // 32 line 3
  _drawIcon(0, 104, VICON32_magnetic, 32, 32, COLOR_GREY);
}
