#include "PA1010D.h"

bool PA1010D::init()
{
  if (_i2cAddress != 0x10) {
    Serial.println(F("Error PA1010D device use I2C address 0x10"));
    return false;
  }

  if (!_gps.begin(_i2cAddress)) {
    Serial.println(F("Error initializing I2C PA1010D device"));
    return false;
  }    

  // turn on RMC (recommended minimum) and GGA (fix data) including altitude
  _gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  
  // 1 Hz update rate
  _gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  _gps.sendCommand(PMTK_API_SET_FIX_CTL_1HZ);
  
  // Request updates on antenna status, comment out to keep quiet
  //_gps.sendCommand(PGCMD_ANTENNA);

  // activate AlwaysLocate mode (dynamic power consumption)
  //delay(1000);
  //_gps.sendCommand("$PMTK225,8*23"); // TODO vasco debug dynamic low consumption (making init fail) after $PMTK225,0*2B OR com:$PMTK225,9*22 ack:$PMTK001,225,3*35

  return true;

}

bool PA1010D::wake()
{
  return true;
}

bool PA1010D::sleep()
{ 
  return true;
}

bool PA1010D::check() 
{
  // TODO vasco move in update fct ?
  char c = _gps.read();
  if (_gps.newNMEAreceived()) {
    _gps.parse(_gps.lastNMEA());
  }

  return false;
}

bool PA1010D::update()
{
  _feed(_data.satellite, _gps.satellites, _satellites, 5);
  _feed(_data.fixQuality, _gps.fixquality, _fixQualities, 3);
  _feed(_data.latitude, _gps.latitudeDegrees, _latitudes, 1);
  _feed(_data.longitude, _gps.longitudeDegrees, _longitudes, 1);
  _feed(_data.altitude, _gps.altitude, _altitudes, 1);
  _feed(_data.speed, _convertToKmH(_gps.speed), _speeds, 1);
  _feed(_data.directionAngle, _gps.angle, _directionAngles, 1);
  _feed(_data.compassAngle, _gps.magvariation, _compassAngles, 1);

  _data.dateTime = _convertToDateTime(_gps.year, _gps.month, _gps.day, _gps.hour, _gps.minute, _gps.seconds);
  _data.latCardinal = _gps.lat;
  _data.longCardinal = _gps.lon;

  return true;
}

long PA1010D::read()
{
  return 0;
}

String PA1010D::_convertToDateTime(int year, int month, int day, int hour, int minute, int second)
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

float PA1010D::_convertToKmH(float knot)
{
  return knot * 1.852; // one knot is 1.852 km/h
}
