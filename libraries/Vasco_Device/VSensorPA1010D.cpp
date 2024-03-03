#include "VSensorPA1010D.h"

void VSensorPA1010D::begin(int addr)
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

    _enabled = true;
  }
}

bool VSensorPA1010D::update(int delay)
{
  // TODO vasco: move in millis loop
  char c = _gps.read();
  if (_gps.newNMEAreceived()) {
    if (!_gps.parse(_gps.lastNMEA())) {
      
      return false;
    }
  }

  if (millis() - _timer > delay) {
    _timer = millis(); // reset the timer
    
    sync();
    
    return true;
  }

  return false;
}

void VSensorPA1010D::sync()
{
  unsigned int time = millis();

  if (_enabled) {
    _setSatellites(_gps.satellites);
    _setFixQuality(_gps.fixquality);
    _setLatitude(_gps.latitudeDegrees);
    _setLongitude(_gps.longitudeDegrees);
    _setAltitude(_gps.altitude);
    _setSpeed(_convertToKmH(_gps.speed));
    _setDirectionAngle(_gps.angle);
    _setCompassAngle(_gps.magvariation);
    _data.dateTime = _convertToDateTime(_gps.year, _gps.month, _gps.day, _gps.hour, _gps.minute, _gps.seconds);
    _data.latCardinal = _gps.lat;
    _data.longCardinal = _gps.lon;
  }

  _processTime = millis() - time;
}

void VSensorPA1010D::sleep(bool isSleeping)
{
  _enabled = !isSleeping;

  if (!_enabled) {
    _data.satellites.status = GRIS;
    _data.fixQuality.status = GRIS;
    _data.latitude.status = GRIS;
    _data.longitude.status = GRIS;
    _data.altitude.status = GRIS;
    _data.speed.status = GRIS;
    _data.directionAngle.status = GRIS;
    _data.compassAngle.status = GRIS;
  }
}

String VSensorPA1010D::_convertToDateTime(int year, int month, int day, int hour, int minute, int second)
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

float VSensorPA1010D::_convertToKmH(float knot)
{
  return knot * 1.852; // one knot is 1.852 km/h
}
