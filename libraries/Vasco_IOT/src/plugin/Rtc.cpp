#include "Rtc.h"

long Rtc::_getTimeStampRTC()
{
  time_t now;
  time(&now);

  return now;
}

void Rtc::_setTimeStampRTC(long timeStamp)
{
  timeval epoch = {timeStamp, 0};

  settimeofday((const timeval*)&epoch, 0);
}

String Rtc::_getDateTimeRTC()
{
  struct tm timeinfo;
  time_t now;
  time(&now);
  localtime_r(&now , &timeinfo);

  char dateTime[_DATETIME_LENGTH];
  snprintf(dateTime, _DATETIME_LENGTH, _DATETIME_FORMAT,
    timeinfo.tm_year + 1900, // in old legacy tm structure, year zero is 1900
    timeinfo.tm_mon + 1, // and months are numbered from 0 to 11
    timeinfo.tm_mday,
    timeinfo.tm_hour,
    timeinfo.tm_min,
    timeinfo.tm_sec
  );

  return dateTime;
}

void Rtc::_setDateTimeRTC(String dateTime)
{
  struct tm timeinfo;
  //struct tm timeinfo = {0, 0, 0, 0, 0, 0, 0, 0, 0};      // Initalize to all zero

  timeinfo.tm_year = dateTime.substring(0,4).toInt() - 1900;
  timeinfo.tm_mon = dateTime.substring(5,7).toInt() - 1;
  timeinfo.tm_mday = dateTime.substring(8,10).toInt();
  timeinfo.tm_hour = dateTime.substring(11,13).toInt();
  timeinfo.tm_min = dateTime.substring(14,16).toInt();
  timeinfo.tm_sec = dateTime.substring(17,19).toInt();
  time_t timeSinceEpoch = mktime(&timeinfo);

  _setTimeStampRTC(timeSinceEpoch);
}

String Rtc::_getUpTimeRTC()
{
  long time = millis();
  int sec = time / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int days = hr / 24; // TODO vasco include days in output

  char uptime[9];
  snprintf(uptime, 9, "%02d:%02d:%02d",
    hr,
    min % 60,
    sec % 60
  );

  return uptime;
}