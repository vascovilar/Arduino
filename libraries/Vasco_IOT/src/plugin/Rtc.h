#ifndef Rtc_h
#define Rtc_h

#include "Arduino.h"
#include "time.h"
#include "sys/time.h"


class Rtc
{
  const char*       _DATETIME_FORMAT = "%04d-%02d-%02d %02d:%02d:%02d";
  static const int  _DATETIME_LENGTH = 20;

  protected:
  
    long    _getTimeRTC(); // timestamp Unix
    void    _setTimeRTC(long timeStamp); // set internal Real Time Clock with timestamp Unix
    String  _getDateTimeRTC(); // return standard string format "2024-01-23 19:34:55"
    void    _setDateTimeRTC(String dateTime); // set RTC with standard string format "2024-01-23 19:34:55"
    String  _getUpTimeRTC(); // get seconds from power on

    // TODO vasco include timezone 

};

#endif
