#include "Container.h"

bool Container::begin(vrun mode)
{
  bool isInit = true;

  for (int i = 0; i < VCHIPSET_COUNT; i++) {
    if (_instance[i]) {
      if (_mode[i] != INIT_ERROR) {
        mode = _mode[i];
      }
      if (!_sequencer[i]->begin(mode)) {
        isInit = false;
      }
    }
  }

  return isInit;
}

bool Container::run()
{
  bool isUpdated = false;

  for (int i = 0; i < VCHIPSET_COUNT; i++) {
    if (_instance[i]) {
      if (_sequencer[i]->run()) {
        _updateChipsetSensors((vchipset)i);
        updated[length++] = (vchipset)i;
        isUpdated = true;
      }
    }
  }
  _processedChecks++;

  if (millis() - _processedTimer > 1000) {
    _processedChecksPerSecond = _processedChecks / ((millis() - _processedTimer) / 1000.0);
    _processedChecks = 0;
    _processedTimer = millis(); // reset timer
  }

  return isUpdated;
}

void Container::bind(Device &device, vrun mode)
{
  vchipset code = device.getChipsetCode();
  _instance[code] = &device;
  _sequencer[code] = new Sequencer(device);
  _mode[code] = mode;
}

void Container::bind(Sensor &sensor, vrun mode)
{
  vchipset code = sensor.getChipsetCode();
  _instance[code] = &sensor;
  _sequencer[code] = new Sequencer(sensor);
  _mode[code] = mode;

  for (int i = 0; i < VSENSOR_COUNT; i++) {
    if (sensor.get((vsensor)i).label != "") {
      // init buffer for each sensor of a chipset
      _buffer[i] = Buffer();
    }
  }
}

vfield Container::getField(vsensor code)
{
  return _field[code];
}

void Container::setField(vsensor code, vfield field)
{
  _buffer[code].push(field.value, _getTimeStampRTC());
  _field[code] = field;
  //Serial.println("Sensor " + field.label + ": " + String(field.value)); // debug
}

Buffer Container::getBuffer(vsensor code)
{
  return _buffer[code];
}

bool Container::isEnabled(vchipset code)
{
  return _sequencer[code]->isEnabled();
}

void Container::_updateChipsetSensors(vchipset code)
{
  if (_instance[code]->isSensor()) {
    Sensor* sensor = (Sensor*)_instance[code];
    for (int i = 0; i < VSENSOR_COUNT; i++) {
      vfield field = sensor->get((vsensor)i);
      if (field.label != "") {
        setField((vsensor)i, field);
      }
    }
  }
}
