#include "Bitmap.h"

void Bitmap::handleAbsoluteValueBar(Screen &screen, int offset, float value, float maximum, float minimum, bool isInBuffer)
{
  int colorbarX = 65;

  float minValuePercent = 2.0;
  float valuePercent = 100 * value / (maximum - minimum);

  screen.rect(colorbarX + 15 * 0, offset, 14, 18, valuePercent > minValuePercent ? COLOR_GREEN: COLOR_BLACK, 4, isInBuffer);
  screen.rect(colorbarX + 15 * 1, offset, 14, 18, valuePercent > 20 ? COLOR_YELLOW: COLOR_BLACK, 4, isInBuffer);
  screen.rect(colorbarX + 15 * 2, offset, 14, 18, valuePercent > 40 ? COLOR_ORANGE: COLOR_BLACK, 4, isInBuffer);
  screen.rect(colorbarX + 15 * 3, offset, 14, 18, valuePercent > 60 ? COLOR_RED: COLOR_BLACK, 4, isInBuffer);
  screen.rect(colorbarX + 15 * 4, offset, 14, 18, valuePercent > 80 ? COLOR_RED: COLOR_BLACK, 4, isInBuffer);
}

void Bitmap::handleStaticGraphPage(Screen &screen, int offset, String name, vcolor code, bool isInBuffer)
{
  int charHeight = 8;
  int baselineY = VSCREEN_HEIGHT - charHeight * 1 - 3;
  int basetextY = baselineY + 3;
  int graphWidth = VSCREEN_WIDTH - 1;

  String label = name;
  for (int i = 0; i < (10 - name.length()); i++) {
    label += " ";
  }

  handlePillTitle(screen, 0, label, "", code, isInBuffer);
  screen.line(0, offset, 0, baselineY, COLOR_GREY_DARK, isInBuffer);
  screen.line(0, baselineY, graphWidth, baselineY, COLOR_GREY_DARK, isInBuffer);
  screen.text("0 ms", 0, basetextY, CHAR_6x8, COLOR_GREY, COLOR_BLACK, isInBuffer);
}

void Bitmap::handleRealtimeGraphUpdate(Screen &screen, int offset, Sensor &sensor, int length, bool isInBuffer)
{
  int   char6x8RightAlignX = VSCREEN_WIDTH - 6 * 5;
  int   char12x16RightAlignX = VSCREEN_WIDTH - 12 * 5;
  int   baselineY = VSCREEN_HEIGHT - 8 * 1 - 3;
  int   basetextY = baselineY + 3;
  int   graphWidth = VSCREEN_WIDTH - 1;
  int   graphHeight = baselineY - offset - 1;

  // harvest data
  float buffer[length];
  float maximum = 0;
  float minimum = 9999999;
  int   time = 0;
  int   timer = micros();
  for (int i = 0; i < length; i++) {
    buffer[i] = sensor.read();
    if (buffer[i] > maximum) {
      maximum = buffer[i];
    }
    if (buffer[i] < minimum) {
      minimum = buffer[i];
    }
  }
  time = micros() - timer;

  // clear graph
  screen.rect(1, offset, graphWidth, graphHeight, COLOR_BLACK, 0, isInBuffer);

  // redraw page variables
  screen.text(String(time / 1000.0), char6x8RightAlignX, basetextY, CHAR_6x8, COLOR_GREY, COLOR_BLACK, isInBuffer);
  screen.text(String(maximum) + "%  ", char12x16RightAlignX, 3, CHAR_12x16, COLOR_GREY, COLOR_BLACK, isInBuffer);
  handleAbsoluteValueBar(screen, 1, maximum, 100, 0, isInBuffer);

  // redraw graph
  float x, y, xo, yo;
  for (int i = 0; i < length; i++) {
    x = i * VSCREEN_WIDTH / (float)length;
    y = _isometric(buffer[i], maximum < 5 ? 5: maximum, minimum > -5 ? -5: minimum, graphHeight - 1, offset);
    if (i != 0) {
      screen.line(xo + 1, yo, x, y, COLOR_WHITE, isInBuffer);
    }
    xo = x;
    yo = y;
  }

  // redraw the zero line
  float zero = _isometric(0, maximum, minimum, graphHeight, offset);
  if (zero > (offset + graphHeight)) {
    zero = offset + graphHeight;
  }
  screen.line(1, zero, graphWidth, zero, COLOR_GREY_DARK, isInBuffer);
}

void Bitmap::handlePillTitle(Screen &screen, int offset, String content, String content2, vcolor code, bool isInBuffer)
{
  int paddingX = 8;
  int paddingY = 2;
  int charWidth = 12;
  int charHeight = 16;
  int textY = offset + paddingY + 1;

  screen.rect(0, offset, (content.length() +  content2.length() + 1) * charWidth + 2 * paddingX, charHeight + 2 * paddingY, code, 8, isInBuffer);
  screen.text(content, paddingX, textY, CHAR_12x16, COLOR_WHITE, code, isInBuffer);
  screen.text(content2, content.length() * (charWidth + 1) + paddingX, textY, CHAR_12x16, COLOR_BLACK, code, isInBuffer);
}