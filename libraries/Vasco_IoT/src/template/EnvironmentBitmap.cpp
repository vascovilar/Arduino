#include "EnvironmentBitmap.h"


// HOME //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EnvironmentBitmap::handleHomePage(String trigram, String ip, String ssid, int offset)
{
  _screen.clear();
  _drawPage(trigram.substring(0, 4), "3");
  _screen.text(VBITMAP_COL_MARGIN, offset, "http://" + ip, SIZE_TEXT, COLOR_WHITE_DARK);
  _screen.text(VBITMAP_COL_MARGIN, offset + VBITMAP_LINESPACING, "réseau " + (ssid == "" ? "aucun": ssid), SIZE_TEXT, COLOR_GREY);

  // planet earth
  //_screen.circle(VSCREEN_WIDTH - 30, offset - 30, 80, COLOR_BLUE); // simple circle 0ko
  _screen.image(183, 20, VIMAGE_earth, 57, 120); // color 1ko

  // stylized separator line
  int limit = 140;
  _screen.line(VBITMAP_COL_PAGE + 1, offset + 2 * VBITMAP_LINESPACING + 8, limit, offset + 2 * VBITMAP_LINESPACING + 8, COLOR_GREY_DARK);
  _screen.line(limit, offset + 2 * VBITMAP_LINESPACING + 8, limit + 10, offset + 2 * VBITMAP_LINESPACING + 18, COLOR_GREY_DARK);
  _screen.line(limit + 10, offset + 2 * VBITMAP_LINESPACING + 18, VSCREEN_WIDTH - 25, offset + 2 * VBITMAP_LINESPACING + 18, COLOR_GREY_DARK);

  int buttonHeight = _screen.height(" ", SIZE_SMALL) + 2 * VBITMAP_PADDING_Y - 1;
  _mouse.clear();
  _mouse.add(_drawButtonMini(VBITMAP_COL_BACK, VSCREEN_HEIGHT - buttonHeight, "EVENT", CURRENT_EVENT));
  _mouse.add(_drawButtonMini(VBITMAP_COL_BACK + 40, VSCREEN_HEIGHT - buttonHeight, "GPS", SAT_TABLE));
}

void EnvironmentBitmap::handleHomeUpdate(String clock, vfield cycles, bool isConnected, int offset)
{
  int paddingToTopRight = VSCREEN_WIDTH - _screen.width(clock, SIZE_TITLE, true) - 24;
  int overflowSecurity = _screen.height(" ", SIZE_TITLE); // security for not showing old number with variable fonts

  // clock
  _screen.rect(paddingToTopRight - overflowSecurity, VBITMAP_PADDING_Y + 1, _screen.width(clock, SIZE_TITLE, true), _screen.height(clock, SIZE_TITLE), COLOR_BLACK);
  _screen.line(paddingToTopRight - overflowSecurity, 10, paddingToTopRight, 10, COLOR_BLUE);
  _screen.text(paddingToTopRight, VBITMAP_PADDING_Y + 1, clock, SIZE_TITLE, COLOR_BLUE, COLOR_BLACK, true);

  // wifi logo
  if (isConnected) {
    _drawIcon(216, -2, VICON24_wifi, 24, 24, COLOR_BLUE, COLOR_BLACK);
  }

  // load : vertical text
  _screen.vertical(0, VSCREEN_HEIGHT - 1, String((int)cycles.value) + " fps", SIZE_SMALL, COLOR_BLUE, COLOR_BLACK);
}

// EVENT //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EnvironmentBitmap::handleEventUpdate(vsensor index, String dateTime, vfield field, Buffer buffer, vcolor bgColor, int offset)
{
  // draw date
  _screen.text(VBITMAP_COL_MARGIN, offset + 5, "News @", SIZE_SMALL, COLOR_GREY);
  _screen.text(VBITMAP_COL_MARGIN + _screen.width("News @", SIZE_SMALL) + 1, offset + 5, dateTime + "  ", SIZE_SMALL, COLOR_BLUE, COLOR_BLACK);

  // add icon sensor 32x32 with round bg
  _screen.circle(VSCREEN_WIDTH - 82, offset - 40, 18, COLOR_BLACK);
  _drawIcon(VSCREEN_WIDTH - 78, offset - 36, _getIcon(index), 32, 32, COLOR_GREY);

  // add history header
  handleHistorySummary(field, buffer.delta, buffer.trend, bgColor, VBITMAP_LINESPACING + offset + 8);
}

// PROBE //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EnvironmentBitmap::handleHistorySummary(vfield field, float delta, byte trend, vcolor bgColor, int offset)
{
  int pillWidth = VSCREEN_WIDTH - VBITMAP_COL_MARGIN;
  int pillHeight = 2 * VBITMAP_LINESPACING + VBITMAP_PADDING_Y;
  int paddingToTitle = VBITMAP_COL_MARGIN + 2 * VBITMAP_RADIUS + VBITMAP_PADDING_X;
  int paddingToUnit = VSCREEN_WIDTH - _screen.width(field.unit, SIZE_TEXT) - VBITMAP_PADDING_X / 2;
  int paddingToValue = paddingToUnit - _screen.width(" " + String(field.value), SIZE_TITLE) - VBITMAP_PADDING_X;
  int offsetFirstLine = offset + VBITMAP_PADDING_Y;
  int offsetSecondLine = offsetFirstLine + VBITMAP_LINESPACING;

  // clear zone
  if (bgColor != COLOR_TRANSPARENT) {
    _screen.rect(VBITMAP_COL_MARGIN, offset, pillWidth, pillHeight, bgColor, VBITMAP_RADIUS);
  }
  // rounded status
  _screen.circle(VBITMAP_COL_MARGIN, offsetFirstLine, VBITMAP_RADIUS, _convert(field.status));

  // title
  if (field.label != "") {
    _screen.text(paddingToTitle, offsetFirstLine, field.label, SIZE_TEXT, COLOR_WHITE_DARK);
  }

  // upper right
  if (field.status != GRIS) {
    // text
    String textLeft = field.text;
    String textRight = String(delta) + " (" + String(field.tolerance) + ")";
    _screen.text(paddingToTitle, offsetSecondLine, textLeft, SIZE_TEXT, COLOR_WHITE_DARK);
    _screen.text(paddingToTitle + _screen.width(textLeft, SIZE_TEXT) + VBITMAP_PADDING_X, offsetSecondLine, textRight, SIZE_TEXT, COLOR_GREY);
    // value
    _screen.text(paddingToValue, offsetFirstLine, " " + String(field.value), SIZE_BIG, COLOR_WHITE, bgColor);
    // unit
    _screen.text(paddingToUnit, offsetFirstLine, field.unit, SIZE_TEXT, COLOR_WHITE_DARK);
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

void EnvironmentBitmap::handleHistoryPage(String title, vfield field, int currentDelay, bool isRealTime, vcolor bgColor, int offset)
{
  _screen.clear();
  _drawPage(title, "", 20);

  // load : vertical text
  _screen.vertical(0, VSCREEN_HEIGHT - 1, "1/" + String((currentDelay / 1000)) + " fps", SIZE_SMALL, COLOR_BLUE, COLOR_BLACK);

  _mouse.clear();
  _mouse.add(_drawButtonArrow(VSCREEN_WIDTH - 30, 0, UP, BUFFER_UP));
  _mouse.add(_drawButtonArrow(VSCREEN_WIDTH - 45, 0, DOWN, BUFFER_DOWN));
  if (isRealTime) {
    _mouse.add(_drawButtonMini(VSCREEN_WIDTH - 80, 4, "LIVE", LIVE_VIEW, true));
  }
  _mouse.add(_drawButtonBack(VBITMAP_COL_BACK, HOME_PAGE));
}

void EnvironmentBitmap::handleHistoryUpdate(vsensor code, vfield field, Buffer buffer, vcolor bgColor, int offset)
{
  // draw header
  handleHistorySummary(field, buffer.delta, buffer.trend, bgColor, offset);

  // draw sensor number
  _screen.rect(VBITMAP_COL_BAR + 21, VBITMAP_PADDING_Y, 12, 16, COLOR_BLACK);
  _screen.text(VBITMAP_COL_BAR + 21, VBITMAP_PADDING_Y, String((int)code), SIZE_TITLE, COLOR_BLUE);

  // draw graph
  int graphOffset = offset + 2 * VBITMAP_LINESPACING + VBITMAP_PADDING_Y;
  _screen.drawValueHistory(VBITMAP_COL_MARGIN, graphOffset, field, buffer, VSCREEN_WIDTH, VSCREEN_HEIGHT - graphOffset - VBITMAP_LINESPACING - 1, COLOR_BLACK);
}

// LIVE //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EnvironmentBitmap::handleRealtimePage(String trigram, vfield field, int offset)
{
  _screen.clear();
  _drawPage(trigram.substring(0, 4), "", 52);

  // title
  _screen.text(VBITMAP_COL_MARGIN, offset, field.label, SIZE_TEXT, COLOR_WHITE_DARK);

  // unit
  _screen.text(VSCREEN_WIDTH - _screen.width(field.unit, SIZE_TEXT), offset, field.unit, SIZE_TEXT, COLOR_GREY);

  // graph limits and zero
  _screen.line(VBITMAP_COL_MARGIN, offset + VBITMAP_LINESPACING, VBITMAP_COL_MARGIN, VBITMAP_OFFSET_FOOTER, COLOR_GREY_DARK);
  _screen.line(VBITMAP_COL_MARGIN, VBITMAP_OFFSET_FOOTER, VSCREEN_WIDTH - 1, VBITMAP_OFFSET_FOOTER, COLOR_GREY_DARK);
  _screen.text(VBITMAP_COL_MARGIN, VBITMAP_OFFSET_FOOTER + VBITMAP_PADDING_Y, "0", SIZE_SMALL, COLOR_GREY);

  _mouse.clear();
  _mouse.add(_drawButtonBack(VBITMAP_COL_BACK, CURRENT_BUFFER));
}

void EnvironmentBitmap::handleRealtimeUpdate(Sensor &sensor, int length, vfield field, int processedChecks, int offset)
{
  int graphOffset = offset + VBITMAP_LINESPACING;
  float offsetBottom = offset + VBITMAP_OFFSET_FOOTER - graphOffset;
  float overflowSecurity = _screen.width(" ", SIZE_BIG, true); // security for not showing old number with varibale fonts
  Snapshot snap = Snapshot(sensor, length);

  _screen.drawSensorSnap(VBITMAP_COL_MARGIN, graphOffset, snap, field.tolerance, field.unit, VSCREEN_WIDTH, VBITMAP_OFFSET_FOOTER - graphOffset, COLOR_BLACK);

  // score @haut-droite
  float paddingToTopRight = VSCREEN_WIDTH - _screen.width(String(snap.score), SIZE_BIG, true);
  _screen.rect(paddingToTopRight - overflowSecurity, VBITMAP_PADDING_Y, VSCREEN_WIDTH - paddingToTopRight, _screen.height(" ", SIZE_BIG), COLOR_BLACK, 0, true);
  _screen.line(paddingToTopRight - overflowSecurity, 10, paddingToTopRight, 10, COLOR_BLUE);
  _screen.text(paddingToTopRight, VBITMAP_PADDING_Y, String(snap.score), SIZE_BIG, COLOR_GREY, COLOR_BLACK, true);

  // processTime @bas-droite
  String delay = String(snap.time) + " ms";
  float paddingToBottomRight = VSCREEN_WIDTH - _screen.width(delay, SIZE_SMALL);
  _screen.rect(paddingToBottomRight - overflowSecurity, offsetBottom + VBITMAP_LINESPACING + 1, VSCREEN_WIDTH, _screen.height(" ", SIZE_SMALL), COLOR_BLACK, 0, true);
  _screen.text(paddingToBottomRight, offsetBottom + VBITMAP_LINESPACING + 1, delay, SIZE_SMALL, COLOR_GREY, COLOR_BLACK, false);

  _drawAnalogBar(VBITMAP_COL_BAR + 5, VBITMAP_PADDING_Y / 2, snap.score, 0, 100, field.tolerance);

  // load : vertical text
  _screen.vertical(0, VSCREEN_HEIGHT - 1, String(processedChecks * length) + " fps", SIZE_SMALL, COLOR_BLUE, COLOR_BLACK);
}

// SAT //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EnvironmentBitmap::handleSatellitePage(String trigram, int offset)
{
  _screen.clear();
  _drawPage(trigram.substring(0, 4), "", 12);
  String titles[7] = {"Date GMT", "Sat.connectes", "Latitude", "Longitude", "Vitesse", "Altitude", "Cap"};
  _drawTableHeader(VBITMAP_COL_MARGIN, offset, titles, 7);

  // load : vertical text
  _screen.vertical(0, VSCREEN_HEIGHT - 1, "1 fps", SIZE_SMALL, COLOR_BLUE, COLOR_BLACK);

  _mouse.clear();
  _mouse.add(_drawButtonBack(VBITMAP_COL_BACK, HOME_PAGE));
}

void EnvironmentBitmap::handleSatelliteUpdate(String dateTime, vfield satellite, float latitude, float longitude, vfield speed, vfield altitude, float cap, vfield fixQuality, int offset)
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
  _drawTableData(VBITMAP_COL_MARGIN, offset, values, 7);

  // status
  vcolor color;
  if (fixQuality.value <= 0) {
    color = COLOR_RED;
  } else if (fixQuality.value <= 1) {
    color = COLOR_YELLOW;
  } else if (fixQuality.value <= 3) {
    color = COLOR_GREEN;
  }
  _drawBullet(VBITMAP_COL_BAR, VBITMAP_PADDING_Y / 2, color);

}
