#include "VSensorLight.h"

int VSensorLight::status()
{
  return _status;
}

void VSensorLight::begin(int addr)
{
  if (addr == 0x60) {
    _uv = Adafruit_SI1145();
    _uv.begin(addr, &Wire);
  }
}

bool VSensorLight::update(int delay)
{
  if (millis() - _timer > delay) {
    _timer = millis(); // reset the timer

    _data.uvIndex = _convertToUvIndex(_uv.readUV());
    _data.visible = _uv.readVisible();
    _data.infraRed = _uv.readIR();

    return true;
  }

  return false;
}

uv_status_code VSensorLight::getUvIndexStatus()
{
  uv_status_code code;

  if (_data.uvIndex <= 2) code = V_LOW_RISK;
  else if (_data.uvIndex <= 5) code = V_MODERATE_RISK;
  else if (_data.uvIndex <= 7) code = V_HIGH_RISK;
  else if (_data.uvIndex <= 10) code = V_VERY_HIGH_RISK;
  else code = V_EXTREME_RISK;

  return code;
}

float VSensorLight::_convertToUvIndex(float uv)
{
  return uv / 100.0;
}
