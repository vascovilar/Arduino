#include "../interface/Sensor.h"

void   Sensor::feed(vfield &field, float value, vlegend* data, int length)
{
  field.value = value;
  for (int i = 0; i < length; i ++) {
    if (value <= data[i].limit || i == length - 1) {
      field.status = data[i].status;
      field.text = data[i].text;
      break;
    }
  }
}