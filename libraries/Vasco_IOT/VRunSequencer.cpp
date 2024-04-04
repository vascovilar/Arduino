#include "VRunSequencer.h" 

bool VRunSequencer::begin(vmode_code mode) 
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

  return _enabled;
}

bool VRunSequencer::run()
{
  long time = esp_timer_get_time();
  int delay;

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

  if (child.check() && _processMode == EVENT_TRIG) {
    delay = 0;
  }
  _checks++;

  if (millis() - _timer >= delay) {
    _updates++;
    _timer = millis();
    if (child.update()) {
      _processedTime = _time / 1000;
      _processedChecks = _checks;
      _processedUpdates = _updates;
      _time = 0;
      _checks = 0;
      _updates = 0;

      return true;
    }
  }

  _time += esp_timer_get_time() - time;

  return false;
}
