#include "Custom_GPS.h"

Custom_GPS::Custom_GPS(int rxPin, int txPin) : ss(rxPin, txPin) {}

void Custom_GPS::begin()
{
    Serial.begin(115200);
    ss.begin(9600);
}

void Custom_GPS::update()
{
    while (ss.available() > 0)
        if (gps.encode(ss.read()))
        {
            if (gps.location.isValid())
            {
                latitude = gps.location.lat();
                lat_str = String(latitude, 6);
                longitude = gps.location.lng();
                lng_str = String(longitude, 6);
            }
            if (gps.date.isValid())
            {
                date_str = "";
                date = gps.date.day();
                month = gps.date.month();
                year = gps.date.year();
                if (date < 10)
                    date_str = '0';
                date_str += String(date);
                date_str += " / ";
                if (month < 10)
                    date_str += '0';
                date_str += String(month);
                date_str += " / ";
                if (year < 10)
                    date_str += '0';
                date_str += String(year);
            }
            if (gps.time.isValid())
            {
                time_str = "";
                hour = gps.time.hour();
                minute = gps.time.minute();
                second = gps.time.second();
                minute = (minute + 30);
                if (minute > 59)
                {
                    minute = minute - 60;
                    hour = hour + 1;
                }
                hour = (hour + 5);
                if (hour > 23)
                    hour = hour - 24;
                if (hour >= 12)
                    pm = 1;
                else
                    pm = 0;
                hour = hour % 12;
                if (hour < 10)
                    time_str = '0';
                time_str += String(hour);
                time_str += " : ";
                if (minute < 10)
                    time_str += '0';
                time_str += String(minute);
                time_str += " : ";
                if (second < 10)
                    time_str += '0';
                time_str += String(second);
                if (pm == 1)
                    time_str += " PM ";
                else
                    time_str += " AM ";
            }
            if (gps.speed.isValid())
            {
                float speed_kmph = gps.speed.kmph();
                speed = speed_kmph;
            }
        }
}

double Custom_GPS::getLatitude()
{
    return latitude;
}

double Custom_GPS::getLongitude()
{
    return longitude;
}

String Custom_GPS::getDate()
{
    return date_str;
}

String Custom_GPS::getTime()
{
    return time_str;
}

double Custom_GPS::getSpeed()
{
    return speed;
}