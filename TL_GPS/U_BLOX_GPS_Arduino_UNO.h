#ifndef U_BLOX_GPS_ARDUINO_UNO_H
#define U_BLOX_GPS_ARDUINO_UNO_H

//#include <Arduino.h>
//#include <SoftwareSerial.h>
#include "TL_Config.h"
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;
//SoftwareSerial GPS(2,3);
  
struct date_time{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
 };

class UBLOX
{
private:
  double lng, lat;
  date_time dt;
  
public:
  UBLOX();
  void read();
  date_time timeData();
  double lngData();
  double latData();
  unsigned char getComma(unsigned char num,const char *str);
  double getDoubleNumber(const char *s);
  double getIntNumber(const char *s);
  void parseGPGGA(const char* GPGGAstr);
};

//extern UBLOX TL_GPS;

#endif
