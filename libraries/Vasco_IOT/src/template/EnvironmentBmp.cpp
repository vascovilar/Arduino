#include "EnvironmentBmp.h"


void EnvironmentBmp::handleHomePage(String trigram, String ip, String ssid, int offset)
{
  _screen.clear();
  _drawPage(trigram.substring(0, 4), "3");
  _screen.text(VSCREEN_COL_MARGIN, offset, "http://" + ip, SIZE_TEXT, COLOR_WHITE_DARK);
  _screen.text(VSCREEN_COL_MARGIN, offset + VSCREEN_LINESPACING, "réseau " + (ssid == "" ? "aucun": ssid), SIZE_TEXT, COLOR_GREY);

  // stylized separator line
  int limit = 140;
  _screen.line(VSCREEN_COL_PAGE + 1, offset + 2 * VSCREEN_LINESPACING + 8, limit, offset + 2 * VSCREEN_LINESPACING + 8, COLOR_GREY_DARK);
  _screen.line(limit, offset + 2 * VSCREEN_LINESPACING + 8, limit + 10, offset + 2 * VSCREEN_LINESPACING + 18, COLOR_GREY_DARK);
  _screen.line(limit + 10, offset + 2 * VSCREEN_LINESPACING + 18, VSCREEN_WIDTH, offset + 2 * VSCREEN_LINESPACING + 18, COLOR_GREY_DARK);

  // planet earth
  _screen.circle(VSCREEN_WIDTH - 30, offset - 30, 80, COLOR_BLUE);

  _mouse.clear();
  _mouse.add(_drawButtonMini(VSCREEN_COL_BACK, VSCREEN_OFFSET_FOOTER, "VOIR", CURRENT_EVENT));
  _mouse.add(_drawButtonMini(VSCREEN_COL_BACK + 34, VSCREEN_OFFSET_FOOTER, "T", SAT_TABLE));
  _mouse.add(_drawButtonMini(VSCREEN_COL_BACK + 53, VSCREEN_OFFSET_FOOTER, "A", EAR_REALTIME));
}

void EnvironmentBmp::handleHomeUpdate(String clock, vfield cycles, bool isConnected, int offset)
{
  int paddingToTopRight = VSCREEN_WIDTH - _screen.width(clock, SIZE_TITLE, true) - 24;
  int overflowSecurity = _screen.height(" ", SIZE_TITLE); // security for not showing old number with variable fonts

  // clock
  _screen.rect(paddingToTopRight - overflowSecurity, VSCREEN_PADDING_Y + 1, _screen.width(clock, SIZE_TITLE, true), _screen.height(clock, SIZE_TITLE), COLOR_BLACK);
  _screen.line(paddingToTopRight - overflowSecurity, 10, paddingToTopRight, 10, COLOR_BLUE);
  _screen.text(paddingToTopRight, VSCREEN_PADDING_Y + 1, clock, SIZE_TITLE, COLOR_BLUE, COLOR_BLACK, true);

  // wifi logo
  if (isConnected) {
    _drawIcon(216, -2, VICON24_wifi, 24, 24, COLOR_CYAN, COLOR_BLACK);
  }

  // load : vertical text
  _screen.vertical(0, VSCREEN_HEIGHT - 1, String((int)cycles.value) + " fps", SIZE_SMALL, COLOR_BLUE, COLOR_BLACK);
}

void EnvironmentBmp::handleEventUpdate(vsensor index, String dateTime, vfield field, Buffer buffer, vcolor bgColor, int offset)
{
  // draw date
  _screen.text(VSCREEN_COL_MARGIN, offset + 5, "News @", SIZE_SMALL, COLOR_GREY);
  _screen.text(VSCREEN_COL_MARGIN + _screen.width("News @", SIZE_SMALL) + 1, offset + 5, dateTime + "  ", SIZE_SMALL, COLOR_BLUE, COLOR_BLACK);

  // add icon sensor 32x32 with round bg
  _screen.circle(VSCREEN_WIDTH - 72, offset - 40, 18, COLOR_BLACK);
  _drawIcon(VSCREEN_WIDTH - 24 - 44, offset - 36, _getIcon(index), 32, 32, COLOR_GREY);

  // add history header
  handleHistorySummary(field, buffer.delta, buffer.trend, bgColor, VSCREEN_LINESPACING + offset + 8);
}

void EnvironmentBmp::handleHistorySummary(vfield field, float delta, byte trend, vcolor bgColor, int offset)
{
  int pillWidth = VSCREEN_WIDTH - VSCREEN_COL_MARGIN;
  int pillHeight = 2 * VSCREEN_LINESPACING + VSCREEN_PADDING_Y;
  int paddingToTitle = VSCREEN_COL_MARGIN + 2 * VSCREEN_RADIUS + VSCREEN_PADDING_X;
  int paddingToUnit = VSCREEN_WIDTH - _screen.width(field.unit, SIZE_TEXT) - VSCREEN_PADDING_X / 2;
  int paddingToValue = paddingToUnit - _screen.width(" " + String(field.value), SIZE_TITLE) - VSCREEN_PADDING_X;
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
    String textLeft = field.text;
    String textRight = String(delta) + " (" + String(field.tolerance) + ")";
    _screen.text(paddingToTitle, offsetSecondLine, textLeft, SIZE_TEXT, COLOR_WHITE_DARK);
    _screen.text(paddingToTitle + _screen.width(textLeft, SIZE_TEXT) + VSCREEN_PADDING_X, offsetSecondLine, textRight, SIZE_TEXT, COLOR_GREY);
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

void EnvironmentBmp::handleHistoryPage(String title, vfield field, int currentDelay, vcolor bgColor, int offset)
{
  _screen.clear();
  _drawPage(title, "", 17);

  // load : vertical text
  _screen.vertical(0, VSCREEN_HEIGHT - 1, "1/" + String((currentDelay / 1000)) + " fps", SIZE_SMALL, COLOR_BLUE, COLOR_BLACK);

  _mouse.clear();
  _mouse.add(_drawButtonArrow(VSCREEN_WIDTH - 30, 2, UP, BUFFER_UP));
  _mouse.add(_drawButtonArrow(VSCREEN_WIDTH - 45, 2, DOWN, BUFFER_DOWN));
  _mouse.add(_drawButtonBack(VSCREEN_COL_BACK, HOME_PAGE));
}

void EnvironmentBmp::handleHistoryUpdate(vsensor code, vfield field, Buffer buffer, vcolor bgColor, int offset)
{
  // draw header
  handleHistorySummary(field, buffer.delta, buffer.trend, bgColor, offset + 5);

  // draw sensor number
  _screen.rect(VSCREEN_COL_BAR + 26, VSCREEN_PADDING_Y, 12, 16, COLOR_BLACK); // 24 is 2 char long
  _screen.text(VSCREEN_COL_BAR + 26, VSCREEN_PADDING_Y, String((int)code), SIZE_TITLE, COLOR_BLUE);

  // draw graph
  _screen.drawValueHistory(field, buffer, offset + 30, VSCREEN_WIDTH, VSCREEN_OFFSET_FOOTER - offset - 30, COLOR_BLACK);
}

void EnvironmentBmp::handleRealtimePage(String trigram, int offset)
{
  _screen.clear();
  _drawPage(trigram.substring(0, 4), "", 65);
  _screen.line(VSCREEN_COL_MARGIN, VSCREEN_OFFSET_HEADER, VSCREEN_COL_MARGIN, VSCREEN_OFFSET_FOOTER, COLOR_GREY_DARK);
  _screen.line(VSCREEN_COL_MARGIN, VSCREEN_OFFSET_FOOTER, VSCREEN_WIDTH - 1, VSCREEN_OFFSET_FOOTER, COLOR_GREY_DARK);
  _screen.text(VSCREEN_COL_MARGIN, VSCREEN_OFFSET_FOOTER + VSCREEN_PADDING_Y, "0", SIZE_SMALL, COLOR_GREY);

  _mouse.clear();
  _mouse.add(_drawButtonBack(VSCREEN_COL_BACK, HOME_PAGE));
}

void EnvironmentBmp::handleRealtimeUpdate(Sensor &sensor, int length, int processedChecks, int offset)
{
  float score = _screen.drawSensorSnap(sensor, length, offset, VSCREEN_WIDTH, VSCREEN_OFFSET_FOOTER - offset, COLOR_BLACK);
  _drawAnalogBar(VSCREEN_COL_BAR, VSCREEN_PADDING_Y / 2, score, 0, 100);

  // load : vertical text
  _screen.vertical(0, VSCREEN_HEIGHT - 1, String(processedChecks * length) + " fps", SIZE_SMALL, COLOR_BLUE, COLOR_BLACK);
}

void EnvironmentBmp::handleSatellitePage(String trigram, int offset)
{
  _screen.clear();
  _drawPage(trigram.substring(0, 4), "", 12);
  String titles[7] = {"Date GMT", "Sat.connectes", "Latitude", "Longitude", "Vitesse", "Altitude", "Cap"};
  _drawTableHeader(VSCREEN_COL_MARGIN, offset, titles, 7);

  // load : vertical text
  _screen.vertical(0, VSCREEN_HEIGHT - 1, "1 fps", SIZE_SMALL, COLOR_BLUE, COLOR_BLACK);

  _mouse.clear();
  _mouse.add(_drawButtonBack(VSCREEN_COL_BACK, HOME_PAGE));
}

void EnvironmentBmp::handleSatelliteUpdate(String dateTime, vfield satellite, float latitude, float longitude, vfield speed, vfield altitude, float cap, vfield fixQuality, int offset)
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
  _drawBullet(VSCREEN_COL_BAR + VSCREEN_PADDING_X, VSCREEN_PADDING_Y / 2, color);

}

void EnvironmentBmp::handleTestPage()
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
