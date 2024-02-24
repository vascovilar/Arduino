#include "VDataLogger.h"


void VDataLogger::begin()
{
  Serial.begin(115200);
  delay(1000);
}

void VDataLogger::println(String text)
{
  _addHistory(text + "\n");
  Serial.println(text);
}

void VDataLogger::print(String text)
{
  _addHistory(text);
  Serial.print(text);
}

String VDataLogger::dump(int delay)
{
  String text;

  for (int i; i < 20; i++) {
    if (millis() - _history[i].time < delay) {
      text += _history[i].text;
    }
  }

  return text;
}

void VDataLogger::_addHistory(String text) 
{
  // fifo = push value in buffer by sliding others from end to start index
  for (int i = 20 - 1; i >= 0; i--) {
    if (i == 0) {
      message message = {
        text,
        millis()
      };
      _history[i] = message;
    } else {
      _history[i] = _history[i-1];
    }
  }
}
