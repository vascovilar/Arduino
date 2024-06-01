#include "../interface/Screen.h"

void Screen::_removeLatinChar(String &content)
{
  content.replace("é", "e");
  content.replace("è", "e");
  content.replace("à", "a");
  content.replace("ç", "c");
  content.replace("ù", "u");
  content.replace("€", "E");
  content.replace("î", "i");
}

float Screen::_isometric(float value, float minimum, float maximum, int width, int offset)
{
  return (1 - ((value - minimum) / (float)(maximum - minimum))) * width + offset;
}

void Screen::drawValueHistory(vfield field, Buffer buffer, int offset, int width2, int height2, vcolor bgColor, bool isInBuffer)
{
  float top = buffer.maximum + field.tolerance;
  float bottom = buffer.minimum - field.tolerance;
  float totalTime = VBUFFER_PUSH_DELAY * VBUFFER_MAX_SIZE / 1000.0;
  float paddingToRight = width2 - 11; // beczuse margin = 5 and arrow size 6
  float offsetBottom = offset + height2;

  if (buffer.length > 0) {
    // clear old graph
    if (bgColor != COLOR_TRANSPARENT) {
      rect(VSCREEN_COL_MARGIN, offset, width2 - VSCREEN_COL_MARGIN, VSCREEN_HEIGHT - offset - 1, bgColor);
    }

    // draw horizontal lines : values max, min, average
    float limits[3] = {buffer.maximum, buffer.minimum, buffer.average};
    for (int i = 0; i < 3; i++) {
      float y = _isometric(limits[i], bottom, top, height2, offset);
      line(VSCREEN_COL_MARGIN, y, paddingToRight, y,  limits[i] == buffer.average ? COLOR_GREY: COLOR_GREY_DARK, isInBuffer);
      text(VSCREEN_COL_MARGIN, y - height(" ", SIZE_SMALL) / 2, String(limits[i]), SIZE_SMALL,  limits[i] == buffer.average ? COLOR_GREY: COLOR_GREY_DARK, bgColor, false, isInBuffer);
    }

    // iterate values to draw graph
    for (int i = 0; i < buffer.length; i++)
    {
      int relativeTime = buffer.timeline[0] - buffer.timeline[i];
      // do not show more than total time window
      if (relativeTime > totalTime) {
        break;
      }

      // calculate coords
      float x1 = (paddingToRight - VSCREEN_COL_MARGIN) * (1 - relativeTime / totalTime) + VSCREEN_COL_MARGIN;

      // draw vertical lines : timeline in minuts (first value is bold line)
      float up = _isometric(top, bottom, top, height2, offset);
      if (i % 20 == 0) {
        line(x1, up, x1, offsetBottom, i == 0 ? COLOR_GREY: COLOR_GREY_DARK, isInBuffer);
        text(x1 - 1, offsetBottom + 1, _convertUpTimeToDateTime(relativeTime, true), SIZE_SMALL, COLOR_GREY, bgColor, false, isInBuffer);
      }

      // calculate coords
      if (i != 0) {
        float x2 = (paddingToRight - VSCREEN_COL_MARGIN) * (1 - (buffer.timeline[0] - buffer.timeline[i - 1]) / totalTime) + VSCREEN_COL_MARGIN;
        float y1 = _isometric(buffer.history[i], bottom, top, height2, offset);
        float y2 = _isometric(buffer.history[i - 1], bottom, top, height2, offset);

        // draw all values
        line(x1, y1, x2, y2, COLOR_WHITE, isInBuffer);
      }
    }

    // draw right arrow (current real time value)
    float y3 = _isometric(field.value, bottom, top, height2, offset);
    if (y3 > offsetBottom - 3) y3 = offsetBottom - 3;
    if (y3 < offset + 3) y3 = offset + 3;
    arrow(paddingToRight, y3, 6, 6, COLOR_WHITE, isInBuffer);
  }
}

float Screen::drawSensorSnap(Sensor &sensor, int length, int offset, int width2, int height2, vcolor bgColor, bool isInBuffer)
{
  float overflowSecurity = width("  ", SIZE_BIG, true); // security for not showing old number with varibale fonts
  float offsetBottom = offset + height2;

  // snapshot data // TODO vasco move to Sensor class
  float buffer[length];
  float maximum = 0;
  float minimum = 9999999;
  int   time = 0;
  int   timer = micros();
  float score;
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
  score = maximum > abs(minimum) ? maximum: abs(minimum);

  // score @haut-droite
  String value = String(score) + "%";
  float paddingToTopRight = width2 - width(value, SIZE_BIG, true);
  rect(paddingToTopRight - overflowSecurity, VSCREEN_PADDING_Y, width2 - paddingToTopRight, height(" ", SIZE_BIG), COLOR_BLACK, 0, true, isInBuffer);
  line(paddingToTopRight - overflowSecurity, 10, paddingToTopRight, 10, COLOR_BLUE, isInBuffer);
  text(paddingToTopRight, VSCREEN_PADDING_Y, value, SIZE_BIG, COLOR_GREY, bgColor, true, isInBuffer);

  // processTime @bas-droite
  String delay = String(time / 1000.0) + " ms";
  float paddingToBottomRight = width2 - width(delay, SIZE_SMALL);
  rect(paddingToBottomRight - overflowSecurity, offsetBottom + VSCREEN_PADDING_Y, width2, height(" ", SIZE_SMALL), COLOR_BLACK, 0, true, isInBuffer);
  text(paddingToBottomRight, offsetBottom + VSCREEN_PADDING_Y, delay, SIZE_SMALL, COLOR_GREY, bgColor, false, isInBuffer);

  // clear old graph
  if (bgColor != COLOR_TRANSPARENT) {
    rect(1, offset, width2 - 1, height2 - 1, COLOR_BLACK, 0, true, isInBuffer);
  }

  // draw graph zero line
  float zero = _isometric(0, minimum > -5 ? -5: minimum, maximum < 5 ? 5: maximum, height2 - 2, offset);
  if (zero > (offsetBottom - 1)) {
    zero = offsetBottom - 1;
  }
  line(1, zero, width2 - 1, zero, COLOR_GREY, isInBuffer);

  // draw graph
  float x, y, xo, yo;
  for (int i = 0; i < length; i++) {
    x = i * width2 / (float)length;
    y = _isometric(buffer[i], minimum > -5 ? -5: minimum, maximum < 5 ? 5: maximum, height2 - 2, offset);
    if (i != 0) {
      line(xo + 1, yo, x, y, COLOR_WHITE, isInBuffer);
    }
    xo = x;
    yo = y;
  }

  return score;
}
