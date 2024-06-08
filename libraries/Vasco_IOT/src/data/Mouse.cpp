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
      if (_zone[i].x1 <= pointer.x && pointer.x <= _zone[i].x2 && _zone[i].y1 <= pointer.y && pointer.y <= _zone[i].y2) { // TODO vasco *** case x1 = 100, x2 = 10
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
  vpointer pointer = _pointer.get();

  /* // copy screen background // TODO vasco dont works beacause cant read screen memory
  _screen.paste(_lastPointer.x, _lastPointer.y, 12, 16);
  _screen.copy(pointer.x, pointer.y, 12, 16); */

  // remove min pixdels
  _screen.bitmap(_lastPointer.x, _lastPointer.y, VICON12_pointer, 12, 16, COLOR_BLACK, COLOR_TRANSPARENT);

  // show mouse pointer
  _screen.bitmap(pointer.x, pointer.y, VICON12_pointer, 12, 16, COLOR_WHITE, COLOR_TRANSPARENT);
  _lastPointer = pointer;
}

void Mouse::clear()
{
  _index = 0;
}
