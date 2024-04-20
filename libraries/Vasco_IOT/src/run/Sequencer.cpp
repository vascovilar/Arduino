#include "Sequencer.h"

bool Sequencer::begin(vrun mode)
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

bool Sequencer::run()
{
  long time = esp_timer_get_time();

  switch (_processMode) {
    case LOW_REFRESH:
      _currentDelay = LONG_UPDATE_TIME;
      break;
    case HIGH_REFRESH:
    case SLEEPING:
      _currentDelay = SHORT_UPDATE_TIME;
      break;
    case EVENT_TRIG:
      _currentDelay = LONG_UPDATE_TIME;
      break;
    case CONTINUOUS:
      _currentDelay = CONTINUOUS_UPDATE_TIME;
      break;
    default:

      return false;
  }

  if (child.check() && _processMode == EVENT_TRIG) {
    _currentDelay = 0;
  }
  _checksBuffer++;

  if (millis() - _timer >= _currentDelay) {
    _updatesBuffer++;
    _timer = millis(); // reset timer
    if (child.update()) {
      _processedTime = _timeBuffer / 1000;
      _processedChecks = _checksBuffer;
      _processedUpdates = _updatesBuffer;
      _timeBuffer = 0;
      _checksBuffer = 0;
      _updatesBuffer = 0;

      return true;
    }
  }

  _timeBuffer += esp_timer_get_time() - time;

  return false;
}