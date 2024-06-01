#include "Bitmap.h" // TODO vasco fix offsets

void Bitmap::handleHomePage(String trigram, String ip, String ssid, int offset)
{
  _screen.clear();
  _drawPage(trigram.substring(0, 4), "3");
  _screen.text(VSCREEN_COL_MARGIN, offset, "http://" + ip, SIZE_TEXT, COLOR_WHITE_DARK);
  _screen.text(VSCREEN_COL_MARGIN, offset + VSCREEN_LINESPACING, "réseau " + (ssid == "" ? "aucun": ssid), SIZE_TEXT, COLOR_GREY);
  // stylized separator line
  int limit = 140;
  _screen.line(1, offset + 2 * VSCREEN_LINESPACING + 5, limit, offset + 2 * VSCREEN_LINESPACING + 5, COLOR_GREY_DARK);
  _screen.line(limit, offset + 2 * VSCREEN_LINESPACING + 5, limit + 10, offset + 2 * VSCREEN_LINESPACING + 15, COLOR_GREY_DARK);
  _screen.line(limit + 10, offset + 2 * VSCREEN_LINESPACING + 15, VSCREEN_WIDTH, offset + 2 * VSCREEN_LINESPACING + 15, COLOR_GREY_DARK);
  // icon32 test
  _drawIcon(VSCREEN_WIDTH - 32 - 30, offset - 10, VICON32_RainAndClouds, 32, COLOR_GREY);

  _mouse.clear();
  _mouse.add(_drawButtonMini(VSCREEN_COL_MARGIN + 0, VSCREEN_OFFSET_FOOTER, "EAR", EAR_REALTIME));
  _mouse.add(_drawButtonMini(VSCREEN_COL_MARGIN + 37, VSCREEN_OFFSET_FOOTER, "EMF", EMF_REALTIME));
  _mouse.add(_drawButtonMini(VSCREEN_COL_MARGIN + 74, VSCREEN_OFFSET_FOOTER, "SAT", SAT_TABLE));
  _mouse.add(_drawButtonMini(VSCREEN_COL_MARGIN + 111, VSCREEN_OFFSET_FOOTER, "GRAPH", BUFFER_GRAPH));
}

void Bitmap::handleHomeUpdate(String clock, vfield cycles, bool isConnected, int offset)
{
  String load = "    " + String((int)cycles.value) + " fps";
  int paddingToTopRight = VSCREEN_WIDTH - _screen.width(clock, SIZE_TITLE, true) - 24;
  int paddingToBottomRight = VSCREEN_WIDTH - _screen.width(load, SIZE_TEXT);
  int overflowSecurity = _screen.height(" ", SIZE_TITLE); // security for not showing old number with varibale fonts

  // clock
  _screen.rect(paddingToTopRight - overflowSecurity, VSCREEN_PADDING_Y + 1, _screen.width(clock, SIZE_TITLE, true), _screen.height(clock, SIZE_TITLE), COLOR_BLACK);
  _screen.line(paddingToTopRight - overflowSecurity, 10, paddingToTopRight, 10, COLOR_BLUE);
  _screen.text(paddingToTopRight, VSCREEN_PADDING_Y + 1, clock, SIZE_BIG, COLOR_BLUE, COLOR_BLACK, true);

  if (isConnected) {
    _drawIcon(216, -2, VICON24_Wifi, 24, COLOR_BLUE, COLOR_BLACK);
  }

  // load
  _screen.text(paddingToBottomRight, VSCREEN_HEIGHT - _screen.height(" ", SIZE_SMALL), load, SIZE_SMALL, COLOR_BLUE, COLOR_BLACK);
}

void Bitmap::handleRealtimePage(String trigram, int offset) // TODO vasco offset is useless, use it !
{
  _screen.clear();
  _drawPage(trigram.substring(0, 4), "", 122);
  _screen.line(0, VSCREEN_OFFSET_FOOTER, 0, VSCREEN_HEIGHT, COLOR_BLACK);
  _screen.line(0, VSCREEN_OFFSET_FOOTER, VSCREEN_WIDTH - 1, VSCREEN_OFFSET_FOOTER, COLOR_GREY_DARK);
  _screen.text(0, VSCREEN_OFFSET_FOOTER + VSCREEN_PADDING_Y, "0", SIZE_SMALL, COLOR_GREY, COLOR_BLACK);

  _mouse.clear();
  _mouse.add(_drawButtonBack(VSCREEN_COL_BACK, HOME_PAGE));
}

void Bitmap::handleRealtimeUpdate(Sensor &sensor, int length, int offset)
{
  float score = _screen.drawSensorSnap(sensor, length, offset, VSCREEN_WIDTH, VSCREEN_OFFSET_FOOTER - offset, COLOR_BLACK);
  _drawAnalogBar(VSCREEN_COL_BAR, VSCREEN_PADDING_Y, score, 0, 100);

}

void Bitmap::handleHistorySummary(vfield field, float delta, byte trend, vcolor bgColor, int offset)
{
  int pillWidth = VSCREEN_WIDTH - VSCREEN_COL_MARGIN;
  int pillHeight = 2 * VSCREEN_LINESPACING + VSCREEN_PADDING_Y;
  int paddingToTitle = VSCREEN_COL_MARGIN + 2 * VSCREEN_RADIUS + VSCREEN_PADDING_X;
  int paddingToUnit = VSCREEN_WIDTH - _screen.width(field.unit, SIZE_TEXT) - VSCREEN_PADDING_X / 2;
  int paddingToValue = paddingToUnit - _screen.width(String(field.value), SIZE_TITLE) - VSCREEN_PADDING_X / 2;
  int offsetFirstLine = offset + VSCREEN_PADDING_Y;
  int offsetSecondLine = offsetFirstLine + VSCREEN_LINESPACING;

  // clear zone
  if (bgColor != COLOR_TRANSPARENT) {
    _screen.rect(VSCREEN_COL_MARGIN, offset, pillWidth, pillHeight, bgColor, VSCREEN_RADIUS);
  }
  // rounded status
  _screen.circle(VSCREEN_COL_MARGIN, offsetFirstLine, VSCREEN_RADIUS, _convert(field.status));
  // title
  if (field.label != "") {
    _screen.text(paddingToTitle, offsetFirstLine, field.label, SIZE_TEXT, COLOR_WHITE_DARK);
  }
  // upper right
  if (field.status != GRIS) {
    // text
    String text = field.text + " ~" + String(delta) + " (" + String(field.tolerance) + ")";
    _screen.text(paddingToTitle, offsetSecondLine, text, SIZE_TEXT, COLOR_GREY);
    // value
    _screen.text(paddingToValue, offsetFirstLine, String(field.value), SIZE_BIG, COLOR_GREY, bgColor);
    // unit
    _screen.text(paddingToUnit, offsetFirstLine, field.unit, SIZE_TEXT, COLOR_GREY);
    // trend
    String symbol = "";
    if (trend == 2) symbol += "-";
    if (trend == 1) symbol += "+";
    _screen.text(paddingToUnit, offsetSecondLine, symbol, SIZE_TEXT, COLOR_GREY, bgColor);

  } else if (field.label != "") {
    // default
    _screen.text(paddingToTitle, offsetSecondLine, "En attente des données...", SIZE_TEXT, COLOR_GREY);
  }
}

void Bitmap::handleHistoryPage(String title, vfield field, Buffer buffer, vcolor bgColor, int offset)
{
  _screen.clear();
  _drawPage(title, "");
  handleHistorySummary(field, buffer.delta, buffer.trend, bgColor, offset);

  _mouse.clear();
  _mouse.add(_drawButtonBack(VSCREEN_COL_BACK, HOME_PAGE));
}

void Bitmap::handleHistoryUpdate(vfield field, Buffer buffer, vcolor bgColor, int offset)
{
  // draw header
  handleHistorySummary(field, buffer.delta, buffer.trend, bgColor, offset);
  // draw graph
  _screen.drawValueHistory(field, buffer, offset + 25, VSCREEN_WIDTH, VSCREEN_OFFSET_FOOTER - offset - 25, COLOR_BLACK);
}

void Bitmap::handleSatellitePage(String trigram, int offset)
{
  _screen.clear();
  _drawPage(trigram.substring(0, 4), "", 70);
  String titles[7] = {"Date GMT", "Sat.connectes", "Latitude", "Longitude", "Vitesse", "Altitude", "Cap"};
  _drawTableHeader(VSCREEN_COL_MARGIN, offset, titles, 7);

  _mouse.clear();
  _mouse.add(_drawButtonBack(VSCREEN_COL_BACK, HOME_PAGE));
}

void Bitmap::handleSatelliteUpdate(String dateTime, vfield satellite, float latitude, float longitude, vfield speed, vfield altitude, float cap, vfield fixQuality, int offset)
{
  // feed table
  String values[7] = {
    dateTime,
    String((int)satellite.value) + " " + fixQuality.text,
    String(latitude, 7),
    String(longitude, 7),
    String(speed.value) + " " + speed.unit,
    String(altitude.value) + " " + altitude.unit,
    String((int)cap) + " degres"
  };
  _drawTableData(VSCREEN_COL_MARGIN, offset, values, 7);

  // status
  vcolor color;
  if (fixQuality.value <= 0) {
    color = COLOR_RED;
  } else if (fixQuality.value <= 1) {
    color = COLOR_YELLOW;
  } else if (fixQuality.value <= 3) {
    color = COLOR_GREEN;
  }
  _drawBullet(VSCREEN_COL_BAR, VSCREEN_PADDING_Y, color);
}

void Bitmap::handleEventUpdate(String dateTime, vfield field, Buffer buffer, vcolor bgColor, int offset)
{
  // draw date
  _screen.text(VSCREEN_COL_MARGIN, offset, dateTime + " ", SIZE_SMALL, COLOR_BLUE, COLOR_BLACK);
  // add history header
  handleHistorySummary(field, buffer.delta, buffer.trend, bgColor, VSCREEN_LINESPACING + offset + 2);
}







/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


vzone Bitmap::_drawPage(String titleLeft, String titleRight, int forcedWidth, vcolor color)
{
  _screen.rect(0, 10, VSCREEN_WIDTH + 20, VSCREEN_HEIGHT + 20, COLOR_BLUE, VSCREEN_RADIUS, false); // + 20 means out of screen voluntary
  _drawPill(2 * VSCREEN_RADIUS, 0, titleLeft, titleRight, color, forcedWidth);

  return (vzone){NONE, 0, 0, VSCREEN_WIDTH, VSCREEN_HEIGHT};
}

vzone Bitmap::_drawPill(float x, float y, String contentLeft, String contentRight, vcolor color, int forcedWidth)
{
  int pillWidth = forcedWidth == 0 ? _screen.width(contentLeft, SIZE_TITLE) + _screen.width(contentRight, SIZE_TITLE) + 4 * VSCREEN_PADDING_X: forcedWidth;
  int pillHeight = _screen.height(contentLeft, SIZE_TITLE) + 2 * VSCREEN_PADDING_Y + 1;
  int pillTextPadding = _screen.width(contentLeft, SIZE_TITLE) + 3 * VSCREEN_PADDING_X;

  _screen.rect(x, y, pillWidth, pillHeight, color, VSCREEN_RADIUS);
  _screen.text(x + VSCREEN_PADDING_X, y + VSCREEN_PADDING_Y, contentLeft, SIZE_TITLE, COLOR_CYAN, color);
  _screen.rect(x + pillTextPadding - VSCREEN_PADDING_X, y, 2, pillHeight, COLOR_BLACK);
  _screen.text(x + pillTextPadding, y + VSCREEN_PADDING_Y, contentRight, SIZE_TITLE, COLOR_BLACK, color);

  return (vzone){NONE, x, y, x + pillWidth, y + pillHeight};
}

vzone Bitmap::_drawButtonMini(float x, float y, String content, vpage target)
{
  int buttonWidth = _screen.width(content, SIZE_SMALL) + 2 * VSCREEN_PADDING_X;
  int buttonHeight = _screen.height(content, SIZE_SMALL) + 2 * VSCREEN_PADDING_Y;
  int paddingToCenter = buttonWidth / 2 - _screen.width(content, SIZE_SMALL) / 2;

  _screen.rect(x, y, buttonWidth, buttonHeight, COLOR_YELLOW, VSCREEN_RADIUS);
  _screen.text(x + paddingToCenter, y + VSCREEN_PADDING_Y, content, SIZE_SMALL, COLOR_BLACK);

  return (vzone){target, x, y, x + buttonWidth, y + buttonHeight};
}

vzone Bitmap::_drawButtonBack(float x, vpage target)
{
  int buttonBackWidth = 60;
  int alignToBottom = VSCREEN_HEIGHT - 1;

  _screen.line(x, alignToBottom, x + buttonBackWidth, alignToBottom, COLOR_YELLOW);

  return (vzone){target, x, alignToBottom, x + buttonBackWidth, alignToBottom};
}

vzone Bitmap::_drawBullet(float x, float y, vcolor color)
{
  int bulletWidth = _screen.width(" ", SIZE_TITLE, true);
  int bulletHeight = _screen.height(" ", SIZE_TITLE);

  _screen.rect(x, y, bulletWidth, bulletHeight, color, VSCREEN_RADIUS / 2);

  return (vzone){NONE, x, y, x + bulletWidth, y + bulletHeight};
}

vzone Bitmap::_drawAnalogBar(float x, float y, float value, float minimum, float maximum)
{
  int bulletWidth = _screen.width(" ", SIZE_TITLE, true);
  int bulletHeight = _screen.height(" ", SIZE_TITLE);
  float valuePercent = 100 * value / (maximum - minimum);

  _drawBullet(x + (bulletWidth + 1) * 0, y, valuePercent > 2 ? COLOR_GREEN: COLOR_BLACK); // + 1 is padding between bullets
  _drawBullet(x + (bulletWidth + 1) * 1, y, valuePercent > 5 ? COLOR_YELLOW: COLOR_BLACK);
  _drawBullet(x + (bulletWidth + 1) * 2, y, valuePercent > 10 ? COLOR_ORANGE: COLOR_BLACK);
  _drawBullet(x + (bulletWidth + 1) * 3, y, valuePercent > 30 ? COLOR_RED: COLOR_BLACK);
  _drawBullet(x + (bulletWidth + 1) * 4, y, valuePercent > 70 ? COLOR_RED: COLOR_BLACK);

  return (vzone){NONE, x, y, x + 5 * (bulletWidth + 1), y + bulletHeight};
}

vzone Bitmap::_drawIcon(float x, float y, const unsigned char* icon, int size, vcolor color, vcolor bgColor)
{
  _screen.bitmap(x, y, icon, size, size, color, bgColor);

  return (vzone){NONE, x, y, x + size, y + size};
}

vzone Bitmap::_drawTableHeader(float x, float y, String* titles, int length)
{
  for (int i = 0; i < length; i++) {
    _screen.text(x, y + 12 * i, titles[i], SIZE_TEXT, COLOR_GREY, COLOR_BLACK);
    _screen.line(x, 10 + y + 12 * i, VSCREEN_WIDTH, 10 + y + 12 * i, COLOR_GREY_DARK);
  }
  // clear last line
  _screen.line(x, 10 + y + 12 * (length - 1), VSCREEN_WIDTH, 10 + y + 12 * (length - 1), COLOR_BLACK);

  return (vzone){NONE, x, y, VSCREEN_WIDTH, y + 12 * length};
}

vzone Bitmap::_drawTableData(float x, float y, String* values, int length)
{
  for (int i = 0; i < length; i++) {
    _screen.text(x + 90, y + 12 * i, String(values[i]), SIZE_TEXT, COLOR_WHITE_DARK, COLOR_BLACK);
  }

  return (vzone){NONE, 100, y, VSCREEN_WIDTH, y + 12 * length};
}
