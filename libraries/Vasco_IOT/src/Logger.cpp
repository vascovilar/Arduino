#include "Logger.h"


bool Logger::begin(vrun mode)
{
  Serial.begin(115200);
  delay(1000);
}

bool Logger::run()
{
  if (millis() - _timer > DEFAULT_UPDATE_TIME) {
    _timer = millis();

    return true;
  }

  return false;
}

void Logger::println(String text)
{
  _addHistory(text + "\n");
  Serial.println(text);
}

void Logger::print(String text)
{
  _addHistory(text);
  Serial.print(text);
}

String Logger::dump(int delay)
{
  String text;

  for (int i; i < 20; i++) {
    if (millis() - _history[i].time < delay) {
      text += _history[i].text;
    }
  }

  return text;
}

void Logger::_addHistory(String text) 
{
  // fifo = push value in buffer by sliding others from end to start index
  for (int i = 20 - 1; i >= 0; i--) {
    if (i == 0) {
      message message = {
        millis(),
        text
      };
      _history[i] = message;
    } else {
      _history[i] = _history[i-1];
    }
  }
}
