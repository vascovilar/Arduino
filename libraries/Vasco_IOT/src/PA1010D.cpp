#include "PA1010D.h"

#define PMTK_SET_NMEA_FULL_POWER "$PMTK225,0*2B"

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

  // Configure GPS: Turn on RMC (minimum) and GGA (fix data) including altitude
  _gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  _gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1hz refresh rate
  _gps.sendCommand(PMTK_API_SET_FIX_CTL_1HZ);
  // Request updates on antenna status, comment out to keep quiet
  //_gps.sendCommand(PGCMD_ANTENNA);

  // TODO vasco why this makes GPS fail after reboot ?
  // Activate 'AlwaysLocate' mode (dynamic power consumption)
  /*
  delay(1000);
  _gps.sendCommand("$PMTK225,8*23");
  if (_gps.read()) {
    if (_gps.newNMEAreceived()) {
      if (_gps.lastNMEA() != "$PMTK001,225,3*35") {
        Serial.println(F("Failed to activate 'AlwaysLocate' mode"));
      }
    }
  }
  */

  return true;
}

bool PA1010D::sleep()
{
  if (_gps.standby()) {
    //_gps.pause(true);

    return true;
  }

  return false;
}

bool PA1010D::wake()
{
  //_gps.pause(false);
  _gps.sendCommand("$PMTK225,0*2B");
  _gps.read();
  if (_gps.newNMEAreceived()) {
    Serial.println(String(_gps.lastNMEA())); // TODO vasco remove println

    return true;
  }

  return false;

  /*if (_gps.wakeup()) { // TODO vasco why bloqued in waitForSentence function ?
    return true;
  }
  return false;*/
}

bool PA1010D::check()
{
  // run as frequently as possible to empty NMEA device buffer avoiding device memory overflow
  // 82 octet by NMEA sentence, 10 time per second = 820 octets
  if (_gps.read()) {
    if (_gps.newNMEAreceived()) {
      _gps.parse(_gps.lastNMEA());
    }
  }

  return false;
}

bool PA1010D::update()
{
  // sensor class values
  feed(_satellite, _gps.satellites, _satellites, 5);
  feed(_fixQuality, _gps.fixquality, _fixQualities, 3);
  feed(_altitude, _gps.altitude, _altitudes, 1);
  feed(_speed, _convertToKmH(_gps.speed), _speeds, 1);

  // update local variables
  _latitude = _gps.latitude; // TODO vasco convert here ddmm.mmmmm
  _longitude = _gps.longitude; // TODO vasco convert here ddmm.mmmmm
  _dateTime = _convertToDateTime(_gps.year, _gps.month, _gps.day, _gps.hour, _gps.minute, _gps.seconds);
  _latCardinal = _gps.lat;
  _longCardinal = _gps.lon;
  _isoLabel = _convertToIsoLabel(_latitude, _longitude, _latCardinal, _longCardinal);
  _compassAngle = _gps.magvariation;
  _directionAngle = _gps.angle;

  return true;
}

float PA1010D::read()
{
  return 0.0;
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

String PA1010D::_convertToIsoLabel(float latitude, float longitude, String latCardinal, String longCardinal)
{
  return String(latitude, 7) + " " + latCardinal + "," + String(longitude, 7) + " " + longCardinal;
}
