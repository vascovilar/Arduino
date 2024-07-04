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

void Screen::drawValueHistory(float x, float y, vfield field, Buffer buffer, int width, int height2, vcolor bgColor)
{
  float top = buffer.maximum + field.tolerance;
  float bottom = buffer.minimum - field.tolerance;
  float totalTime = VBUFFER_PUSH_DELAY * VBUFFER_MAX_SIZE / 1000.0;
  float paddingToRight = width - 11; // because margin = 5 and arrow size 6
  float offsetBottom = y + height2;

  if (buffer.length > 0) {
    // clear old graph
    if (bgColor != COLOR_TRANSPARENT) {
      rect(x, y, width - x, VSCREEN_HEIGHT - y - 1, bgColor);
    }

    // draw horizontal lines : values max, min, average
    float limits[3] = {buffer.maximum, buffer.minimum, buffer.average};
    for (int i = 0; i < 3; i++) {
      float y1 = _isometric(limits[i], bottom, top, height2, y);
      line(x, y1, paddingToRight, y1,  limits[i] == buffer.average ? COLOR_GREY: COLOR_GREY_DARK);
      text(x, y1 - height(" ", SIZE_SMALL) / 2, String(limits[i]), SIZE_SMALL,  limits[i] == buffer.average ? COLOR_GREY: COLOR_GREY_DARK, bgColor, false);
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
      float x1 = (paddingToRight - x) * (1 - relativeTime / totalTime) + x;

      // draw vertical lines : timeline in minuts (first value is bold line)
      float up = _isometric(top, bottom, top, height2, y);
      if (i % 30 == 0) {
        line(x1, up, x1, offsetBottom, i == 0 ? COLOR_GREY: COLOR_GREY_DEEPDARK);
        text(x1 - 1, offsetBottom + 1, _convertUpTimeToDateTime(relativeTime, true), SIZE_SMALL, COLOR_GREY_DARK, bgColor, false);
      }

      // calculate coords
      if (i != 0) {
        float x2 = (paddingToRight - x) * (1 - (buffer.timeline[0] - buffer.timeline[i - 1]) / totalTime) + x;
        float y1 = _isometric(buffer.history[i], bottom, top, height2, y);
        float y2 = _isometric(buffer.history[i - 1], bottom, top, height2, y);

        // draw all values
        line(x1, y1, x2, y2, COLOR_WHITE);
      }
    }

    // draw right arrow (current real time value)
    float y3 = _isometric(field.value, bottom, top, height2, y);
    if (y3 > offsetBottom - 3) y3 = offsetBottom - 3;
    if (y3 < y + 3) y3 = y + 3;
    arrow(paddingToRight, y3, 6, 6, COLOR_WHITE, RIGHT);
  }
}

void Screen::drawSensorSnap(float x, float y, Snapshot snap, float tolerance, String unit, int width, int height, vcolor bgColor)
{
  float offsetBottom = y + height;

  // clear old graph
  if (bgColor != COLOR_TRANSPARENT) {
    rect(x + 1, y, width - x - 1, height - 1, COLOR_BLACK, 0, true);
  }

  // distribution graph vertical line
  int distributionWidth = 30;
  line(width - distributionWidth, y, width - distributionWidth, y + height, COLOR_GREY_DARK);

  // draw graph zero line
  float zero = _isometric(0, snap.minimum > -tolerance ? -tolerance: snap.minimum, snap.maximum < tolerance ? tolerance: snap.maximum, height - 2, y);
  if (zero > (offsetBottom - 1)) {
    zero = offsetBottom - 1;
  }
  line(x, zero, width - distributionWidth, zero, COLOR_GREY);

  // draw average
  float moy = _isometric(snap.average, snap.minimum, snap.maximum, height - 2, y);
  line(x, moy, width - distributionWidth, moy, COLOR_GREY_DARK);

  // draw distribution graph
  float x1, y1, xo, yo;
  for (int i = 0; i < snap.length; i++) {
    x1 = x + 1 + i * (width - x - distributionWidth - 5) / (float)snap.length;
    y1 = _isometric(snap.buffer[i], snap.minimum > -tolerance ? -tolerance: snap.minimum, snap.maximum < tolerance ? tolerance: snap.maximum, height - 2, y);
    if (i != 0) {
      line(xo + 1, yo, x1 + 1, y1, COLOR_WHITE);
      point(width - random(1, distributionWidth), yo, COLOR_GREY);
    }
    xo = x1;
    yo = y1;
  }
}
