#include "Sequencer.h"

bool Sequencer::begin(vrun mode)
{
  if (!_enabled) {
    // init while not enabled
    _enabled = _child.init();
  }

  // switch to mode
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
    } else if (_processMode != PAUSED) {
      // change mode. Mode PAUSED stays locked to pause anyway
      _processMode = mode;
    }
  }

  return _enabled;
}

bool Sequencer::run()
{
  long time = esp_timer_get_time();

  switch (_processMode) {
    case LOW_FREQUENCY:
    case EVENT_TRIG:
      _currentDelay = VRUN_LONG_DELAY;
      break;
    case HIGH_FREQUENCY:
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
  if (isTime(_currentDelay)) {
    _updatesBuffer++;

    // trig custom function and update local counters
    if (_child.update()) {
      _isSomethingNew = true;
      _timeBuffer += esp_timer_get_time() - time;

      // update processed and reset counters
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

void Sequencer::pause()
{
  // save current mode and change mode
  _processModeSave = _processMode;
  _processMode = PAUSED;
}

void Sequencer::resume()
{
  // restore saved mode for paused chipsets
  if (_processMode == PAUSED) {
    _processMode = _processModeSave;
  }
}

bool Sequencer::popSomethingNew()
{
  if (_isSomethingNew) {
    _isSomethingNew = false;

    return true;
  }

  return false;
}
