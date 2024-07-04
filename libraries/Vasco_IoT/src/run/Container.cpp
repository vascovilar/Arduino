#include "Container.h"

bool Container::begin(vrun mode)
{
  bool isInit = true;

  for (int i = 0; i < VCHIPSET_COUNT; i++) {
    if (_instance[i]) {
      vrun run = mode;
      // if initially setted, modifiy input mode variable
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
        if (_instance[(vchipset)i]->isSensor()) {
          // update local variable field and buffer (Sensors only)
          _updateLocalSensorsValues((vchipset)i);
          // set flag to harvest laster
          somethingUpdated = true;
        }
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

    return _sequencer[(int)code]->popSomethingNew();
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

bool Container::isEnabled(vchipset code)
{
  if (_instance[code]) {

    return _sequencer[code]->isEnabled();
  }

  return false;
}

bool Container::isSensor(vchipset code)
{
  if (_instance[code]) {

    return _instance[code]->isSensor();
  }

  return false;
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

int Container::getCurrentDelay(vchipset code)
{
  if (_instance[code]) {

    return _sequencer[code]->getCurrentDelay();
  }

  return 0;
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

  return 0;
}

vcodes Container::getBindedCodes()
{
  vcodes list = {{}, 0, {}, 0};

  for (int i = 0; i < VCHIPSET_COUNT; i++) {
    if (_instance[i]) {
      list.chipsetCodes[list.chipsetLength++] = (vchipset)i;
      if (_instance[i]->isSensor()) {
        Sensor* sensor = (Sensor*)_instance[i];
        for (int j = 0; j < VSENSOR_COUNT; j++) {
          vfield field = sensor->get((vsensor)j);
          if (field.label != "") {
            list.sensorCodes[list.sensorLength++] = (vsensor)j;
          }
        }
      }
    }
  }

  return list;
}

vdatatables Container::getChipsetDataTable(vchipset* chipsetCodes, int chipsetLength)
{
  vdatatables table = {};

  // header
  table.lines[table.length++] = (vdatatable){"Chipset", "Programme", "Délai m.a.j", "Mode", "Capteur"};
  // other lines
  for (int i = 0; i < chipsetLength; i++) {
    if (_instance[chipsetCodes[i]]) {
      table.lines[table.length++] = (vdatatable){
        vchipsetName[chipsetCodes[i]],
        String(_sequencer[chipsetCodes[i]]->getProcessedChecks()) + " frames",
        String(_sequencer[chipsetCodes[i]]->getCurrentDelay()) + " ms",
        vrunName[_sequencer[chipsetCodes[i]]->getCurrentMode()],
        _instance[chipsetCodes[i]]->isSensor() ? "oui": "",
      };
    }
  }

  return table;
}

vdatatables Container::getSensorDataTable(vsensor* sensorCodes, int sensorLength)
{
  vdatatables table;


  return table;
}

vfield Container::getWorstField()
{
  vstatus status = GRIS;
  vfield worst;
  vcodes list = getBindedCodes();

  for (int i = 0; i < list.sensorLength; i++) {
    vfield field = getField(list.sensorCodes[i]);
    if (field.label != "Pression") { // TODO vasco remove this exception when resolved
      if (field.status > status) {
        status = field.status;
        worst = field;
      }
    }
  }

  return worst;
}

void Container::setField(vsensor code, vfield field)
{
  _buffer[code].push(field.value, getTimeStamp());
  _field[code] = field;
  //Serial.println("Sensor " + field.label + ": " + String(field.value)); // debug
}

void Container::_updateLocalSensorsValues(vchipset code)
{
  Sensor* sensor = (Sensor*)_instance[code];
  for (int i = 0; i < VSENSOR_COUNT; i++) {
    vfield field = sensor->get((vsensor)i);
    if (field.label != "" && field.status > GRIS) { // >GRIS to avoid storing zeros
      // try get each sensor code in a sensor instance
      setField((vsensor)i, field);
    }
  }
}
