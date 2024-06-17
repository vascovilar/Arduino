#include "Container.h"

bool Container::begin(vrun mode)
{
  bool isInit = true;

  for (int i = 0; i < VCHIPSET_COUNT; i++) {
    if (_instance[i]) {
      vrun run = mode;
      // if already init-setted modifiy input mode variable
      if (_initialMode[i] != INIT_ERROR) {
        run = _initialMode[i];
      }
      // initialize chipset
      if (!_sequencer[i]->begin(run)) {
        isInit = false;
      }
    }
  }

  return isInit;
}

bool Container::run()
{
  bool somethingUpdated = false;

  for (int i = 0; i < VCHIPSET_COUNT; i++) {
    if (_instance[i]) {
      // if something new
      if (_sequencer[i]->run()) {
        // update local variable field and buffer (Sensors only)
        _updateLocalSensorsValues((vchipset)i);
        // set flag to harvest laster
        somethingUpdated = true;
      }
    }
  }

  _checksBuffer++;
  if (isTime(VRUN_SECOND_DELAY)) {
    _processedChecks = _checksBuffer;
    _checksBuffer = 0;
  }

  return somethingUpdated;
}

void Container::bind(Device &device, vrun mode)
{
  vchipset code = device.getChipsetCode();
  _instance[code] = &device;
  _sequencer[code] = new Sequencer(device);
  _initialMode[code] = mode;
}

void Container::bind(Sensor &sensor, vrun mode)
{
  vchipset code = sensor.getChipsetCode();
  _instance[code] = &sensor;
  _sequencer[code] = new Sequencer(sensor);
  _initialMode[code] = mode;

  for (int i = 0; i < VSENSOR_COUNT; i++) {
    vfield field = sensor.get((vsensor)i);
    if (field.label != "") {
      // init buffer for each sensor of a chipset
      _buffer[i] = Buffer();
      _field[i] = field;
    }
  }
}

bool Container::changed(vchipset code)
{
  if (_instance[code]) {

    return _sequencer[(int)code]->isSomethingNew();
  }

  return false;
}

void Container::pause()
{
  for (int i = 0; i < VCHIPSET_COUNT; i++) {
    if (_instance[i]) {
      if (_initialMode[i] != AWARE) {
        // pause each available chipset not in AWARE mode initially
        _sequencer[i]->pause();
      }
    }
  }
}

void Container::resume()
{
  for (int i = 0; i < VCHIPSET_COUNT; i++) {
    if (_instance[i]) {
      // unpause each available chipset
      _sequencer[i]->resume();
    }
  }
}

int Container::getCurrentDelay(vsensor code)
{
  for (int i = 0; i < VCHIPSET_COUNT; i++) {
    if (_instance[i]) {
      if (_instance[i]->isSensor()) {
        Sensor* sensor = (Sensor*)_instance[i];
        vfield field = sensor->get(code);
        if (field.label != "") {

          return _sequencer[i]->getCurrentDelay();
        }
      }
    }
  }

  return 0; // never goes here but needed for compilator :-/
}

bool Container::isRealTime(vsensor code)
{
  for (int i = 0; i < VCHIPSET_COUNT; i++) {
    if (_instance[i]) {
      if (_instance[i]->isSensor()) {
        Sensor* sensor = (Sensor*)_instance[i];
        vfield field = sensor->get(code);
        if (field.label != "") {

          return sensor->isRealTime();
        }
      }
    }
  }

  return false;
}


bool Container::isEnabled(vchipset code)
{
  if (_instance[code]) {

    return _sequencer[code]->isEnabled();
  }

  return false;
}

void Container::setField(vsensor code, vfield field)
{
  _buffer[code].push(field.value, getTimeStamp());
  _field[code] = field;
  //Serial.println("Sensor " + field.label + ": " + String(field.value)); // debug
}

void Container::_updateLocalSensorsValues(vchipset code)
{
  if (_instance[code]->isSensor()) {
    Sensor* sensor = (Sensor*)_instance[code];
    for (int i = 0; i < VSENSOR_COUNT; i++) {
      vfield field = sensor->get((vsensor)i);
      if (field.label != "" && field.status > GRIS) { // >GRIS to avoid storing zeros
        // try get each sensor code in a sensor instance
        setField((vsensor)i, field);
      }
    }
  }
}
