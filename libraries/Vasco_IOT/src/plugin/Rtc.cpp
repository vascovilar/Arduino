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
  long now = _getTimeStampRTC();

  return _convertTimeStampToDateTimeRTC(now);
}

void Rtc::_setDateTimeRTC(String dateTime)
{
  long timeStamp = _convertDateTimeToTimeStampRTC(dateTime);
  _setTimeStampRTC(timeStamp);
}

String Rtc::_getUpTimeRTC()
{
  long upTime = millis() / 1000;

  return _convertUpTimeToDateTimeRTC(upTime);
}

String Rtc::_convertTimeStampToDateTimeRTC(long timeStamp)
{
  struct tm timeinfo;
  localtime_r(&timeStamp, &timeinfo); // convert unix timeStamp to timeinfo var containing year, mon, day, ...

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

long Rtc::_convertDateTimeToTimeStampRTC(String dateTime)
{
  struct tm timeinfo;
  //struct tm timeinfo = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // Initalize to all zero ?

  timeinfo.tm_year = dateTime.substring(0,4).toInt() - 1900;
  timeinfo.tm_mon = dateTime.substring(5,7).toInt() - 1;
  timeinfo.tm_mday = dateTime.substring(8,10).toInt();
  timeinfo.tm_hour = dateTime.substring(11,13).toInt();
  timeinfo.tm_min = dateTime.substring(14,16).toInt();
  timeinfo.tm_sec = dateTime.substring(17,19).toInt();

  return mktime(&timeinfo);
}

String Rtc::_convertUpTimeToDateTimeRTC(int upTime, bool isShort)
{
  int sec = upTime;
  int min = sec / 60;
  int hr = min / 60;
  int days = hr / 24;

  char dateTime[13];
  if (isShort) {
    if (upTime < 60) {
      snprintf(dateTime, 3, "%02d", sec % 60);
    } else if (upTime < 3600) {
      snprintf(dateTime, 6, "%02d:%02d", min % 60, sec % 60);
    } else {
      snprintf(dateTime, 6, "%02d:%02d", hr % 24, min % 60);
    }
  } else {
    snprintf(dateTime, 13, "%d %02d:%02d:%02d", days, hr % 24, min % 60, sec % 60);
  }

  return dateTime;
}
