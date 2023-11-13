#ifndef Custom_GPS_h
#define Custom_GPS_h

#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

class Custom_GPS
{
public:
    Custom_GPS(int rxPin, int txPin);
    void begin();
    void update();

    double getLatitude();
    double getLongitude();
    String getDate();
    String getTime();
    double getSpeed();

private:
    TinyGPSPlus gps;
    SoftwareSerial ss;
    double latitude, longitude, speed;
    int year, month, date, hour, minute, second;
    String date_str, time_str, lat_str, lng_str;
    int pm;
};

#endif
