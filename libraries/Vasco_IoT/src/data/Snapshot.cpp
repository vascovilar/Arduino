#include "Snapshot.h"

void Snapshot::_hydrate(int length2)
{
  if (length2 > VSNAPSHOT_MAX_SIZE) {
    length2 = VSNAPSHOT_MAX_SIZE;
  }

  for (int i = 0; i < length2; i++) {
    buffer[i] = _sensor.read();
    if (buffer[i] > maximum) {
      maximum = buffer[i];
    }
    if (buffer[i] < minimum) {
      minimum = buffer[i];
    }
    length++;
  }

  time = (micros() - _timer) / 1000.0;
  score = maximum > abs(minimum) ? maximum: abs(minimum);
}