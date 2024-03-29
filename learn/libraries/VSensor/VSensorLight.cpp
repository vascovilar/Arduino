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

    _data.uvIndex = _convertToUVIndex(_uv.readUV());
    _data.visible = _uv.readVisible() - 260.0;
    _data.infraRed = _uv.readIR() - 252.0;

    return true;
  }

  return false;
}

uv_status_code VSensorLight::getUVIndexStatus()
{
  uv_status_code code;

  if (_data.uvIndex <= 2) code = V_LOW_RISK;
  else if (_data.uvIndex <= 5) code = V_MODERATE_RISK;
  else if (_data.uvIndex <= 7) code = V_HIGH_RISK;
  else if (_data.uvIndex <= 10) code = V_VERY_HIGH_RISK;
  else code = V_EXTREME_RISK;

  return code;
}

float VSensorLight::_convertToUVIndex(float uv)
{
  return uv / 100.0;
}
