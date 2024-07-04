#include "Snapshot.h"

void Snapshot::_hydrate(int length2)
{
  if (length2 > VSNAPSHOT_MAX_SIZE) {
    length2 = VSNAPSHOT_MAX_SIZE;
  }

  float total = 0;
  for (int i = 0; i < length2; i++) {
    float value = _sensor.read();

    buffer[i] = value;
    length++;
    total += value;

    if (buffer[i] > maximum) {
      maximum = buffer[i];
    }
    if (buffer[i] < minimum) {
      minimum = buffer[i];
    }

  }

  average = total / length;
  time = (micros() - _timer) / 1000.0;
  score = maximum > abs(minimum) ? maximum: abs(minimum);
}