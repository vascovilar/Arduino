#include "Mouse.h"

void Mouse::add(vzone zone)
{
  _zone[_index] = zone;
  _index++;
}

vpage Mouse::hover()
{

  return (vpage){};
}

vpage Mouse::click()
{
  vpointer pointer = _pointer.get();

  // test position if click
  if (pointer.click) {
    for (int i = 0; i < _index; i++) {
      if (_zone[i].x1 <= pointer.x && pointer.x <= _zone[i].x2 && _zone[i].y1 <= pointer.y && pointer.y <= _zone[i].y2) { // TODO vasco case x1 = 100, x2 = 10
        // avoid bug : repeating click if zones are superposing
        pointer.click = false;
        _pointer.set(pointer);

        return _zone[i].page;
      }
    }
  }

  return NONE;
}

void Mouse::draw()
{
  // draw cursor
  vpointer pointer = _pointer.get();

  _screen.point(_lastPointer.x, _lastPointer.y, COLOR_BLACK); // TODO vasco make with etft_Sprite
  _screen.point(pointer.x, pointer.y, COLOR_WHITE);
  _lastPointer = pointer;
}

void Mouse::clear()
{
  _index = 0;
}
