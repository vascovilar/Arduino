#include "VSensorGPS.h"

void VSensorGPS::begin(int addr)
{
  if (addr == 0x10) {
    _gps = Adafruit_GPS(&Wire);
    _gps.begin(addr);

    // turn on RMC (recommended minimum) and GGA (fix data) including altitude
    _gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    
    // 1 Hz update rate
    _gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    _gps.sendCommand(PMTK_API_SET_FIX_CTL_1HZ);
    
    // Request updates on antenna status, comment out to keep quiet
    //_gps.sendCommand(PGCMD_ANTENNA);

    // activate AlwaysLocate mode (dynamic power consumption)
    _gps.sendCommand("$PMTK225,8*23");
  }
}

bool VSensorGPS::update(int delay)
{
  // TODO vasco: move in millis loop
  // update in loop
  char c = _gps.read();
  if (_gps.newNMEAreceived()) {
    if (!_gps.parse(_gps.lastNMEA())) {
      
      return false;
    }
  }

  // trig data reading on sensor
  if (millis() - _timer > delay) {
    _timer = millis(); // reset the timer
    
    _data.dateTime = _convertToDateTime(_gps.year, _gps.month, _gps.day, _gps.hour, _gps.minute, _gps.seconds);
    _data.fixQuality = _convertToQualityLabel(_gps.fixquality);
    _data.satellites = _gps.satellites;
    _data.latitude = _gps.latitudeDegrees;
    _data.latCardinal = _gps.lat;
    _data.longitude = _gps.longitudeDegrees;
    _data.longCardinal = _gps.lon;
    _data.altitude = _gps.altitude;
    _data.speed = _convertToKmH(_gps.speed);
    _data.directionAngle = _gps.angle;
    _data.compassAngle = _gps.magvariation;
    _data.comCardinal = _gps.mag;
    
    return true;
  }

  return false;
}

String VSensorGPS::getIsoDecimalLabel()
{
  return String(_data.latitude, 7) + " " + _data.latCardinal + ","
       + String(_data.longitude, 7) + " " + _data.longCardinal;
}

String VSensorGPS::_convertToDateTime(int year, int month, int day, int hour, int minute, int second)
{
  String output = "20";

  if (year < 10) { output += "0"; }
  output += String(year) + "-";
  if (month < 10) { output += "0"; }
  output += String(month) + "-";
  if (day < 10) { output += "0"; }
  output += String(day) + " ";
  if (hour < 10) { output += "0"; }
  output += String(hour) + ":";
  if (minute < 10) { output += "0"; }
  output += String(minute) + ":";
  if (second < 10) { output += "0"; }
  output += String(second);

  return output;
}

String VSensorGPS::_convertToQualityLabel(int quality)
{
  String output = "";

  if (_gps.fixquality == 2) {
    output += "DGPS";
  } else if(_gps.fixquality == 1) {
    output += "GPS";
  }

  return output;
}

float VSensorGPS::_convertToKmH(float knot)
{
  return knot * KNOTS_TO_KMH;
}
