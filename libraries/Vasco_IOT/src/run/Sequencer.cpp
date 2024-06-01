#include "Sequencer.h"

bool Sequencer::begin(vrun mode)
{
  if (!_enabled) {
    // init while not enabled
    _enabled = _child.init();
  }
  if (_enabled) {
    if (mode == SLEEPING) {
      // call custom function then change mode
      if (_child.sleep()) {
        _processMode = SLEEPING;
      }
    } else if (_processMode == SLEEPING) {
      // call custom function then change mode
      if (_child.wake()) {
        _processMode = mode;
      }
    } else {
      // not knows mode change mode anyways
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
    case EVENT_TRIG:
      _currentDelay = VRUN_LONG_DELAY;
      break;
    case HIGH_REFRESH:
      _currentDelay = VRUN_SHORT_DELAY;
      break;
    case EACH_SECOND:
      _currentDelay = VRUN_SECOND_DELAY;
      break;
    case AWARE:
      _currentDelay = VRUN_AWARE_DELAY;
      break;
    default:
      // ever false for other modes
      return false;
  }

  // called each round
  if (_child.check() && _processMode == EVENT_TRIG) {
    _currentDelay = 0;
  }
  _checksBuffer++;

  // called by periodic trig
  if (millis() - _timer >= _currentDelay) {
    _timer = millis(); // reset timer
    _updatesBuffer++;
    // trig custom function and update local counters
    if (_child.update()) {
      _processedTime = _timeBuffer / 1000;
      _processedChecks = _checksBuffer;
      _processedUpdates = _updatesBuffer;
      _timeBuffer = 0;
      _checksBuffer = 0;
      _updatesBuffer = 0;

      _isSomethingNew = true;
    }
  }

  // increment process time containing calls to check and update chipset functions
  _timeBuffer += esp_timer_get_time() - time;

  return _isSomethingNew;
}

void Sequencer::pause()
{
  // save current mode and change mode
  _processModeSave = _processMode;
  _processMode = PAUSE;
}

void Sequencer::resume()
{
  // restore saved mode for paused chipsets
  if (_processMode == PAUSE) {
    _processMode = _processModeSave;
  }
}

bool Sequencer::isSomethingNew()
{
  if (_isSomethingNew) {
    _isSomethingNew = false;

    return true;
  }

  return false;
}
