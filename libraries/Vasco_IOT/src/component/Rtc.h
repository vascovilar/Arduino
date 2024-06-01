#ifndef Rtc_h
#define Rtc_h

#include "Arduino.h"
#include "time.h"
#include "sys/time.h"


class Rtc
{
  const char*       _DATETIME_FORMAT = "%04d-%02d-%02d %02d:%02d:%02d";
  static const int  _DATETIME_LENGTH = 20;

  public:

    long    getTimeStamp(); // timestamp Unix
    void    setTimeStamp(long timeStamp); // set internal Real Time Clock with timestamp Unix
    String  getDateTime(); // return standard string format "2024-01-23 19:34:55"
    void    setDateTime(String dateTime); // set RTC with standard string format "2024-01-23 19:34:55"
    String  getUpTime(); // get seconds from power on

  protected:

    String  _convertTimeStampToDateTime(long timeStamp); // in seconds
    long    _convertDateTimeToTimeStamp(String dateTime); // string format "2024-01-23 19:34:55"
    String  _convertUpTimeToDateTime(int upTime, bool isShort = false); // in seconds

    // TODO vasco include timezone

};

#endif
