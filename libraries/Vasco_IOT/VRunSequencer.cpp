#include "VRunSequencer.h" 


void VRunSequencer::begin(vmode_code mode) 
{
  if (! _enabled) {
    _enabled = child.init();
  } 
  if (_enabled) {
    if (mode == SLEEPING) {  
      if (child.sleep()) {
        _processMode = SLEEPING;
      }
    } else if (_processMode == SLEEPING) {
      if (child.wake()) {
        _processMode = mode;
      }
    } else {
      _processMode = mode;
    }
  } 
}

bool VRunSequencer::update()
{
  unsigned int delay;

  switch (_processMode) {
    case LOW_REFRESH:
      delay = LONG_UPDATE_TIME;
      break;
    case HIGH_REFRESH:
      delay = SHORT_UPDATE_TIME;
      break;
    case EVENT_TRIG:
      delay = LONG_UPDATE_TIME;
      break;
    default:

      return false;
  }

  // TODO vasco: additionnal processTime in check and read fct
  float value = child.check();
  if (value > child.maxValue) {
    child.maxValue = value;
  }

  if (_processMode == EVENT_TRIG && child.event()) {
    delay = 0;
  }

  if (millis() - _timer > delay) {
    _timer = millis();
    if (child.sync()) {
      child.maxValue = 0;
      _processTime = millis() - _timer;

      return true;
    }
  }

  return false;
}

float VRunSequencer::read()
{  
  return 0.0;
}
