#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(4, 5); // The serial connection to the GPS device
float latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;
void setup(){
  Serial.begin(115200);
  ss.begin(9600);
  Serial.println();
}
void loop(){
  while (ss.available() > 0)
    if (gps.encode(ss.read())){
      if (gps.location.isValid()){
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        Serial.print("longitude:");
        Serial.println(longitude);
        Serial.print("latitude:");
        Serial.println(latitude);
      }
      if (gps.date.isValid()){
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
        Serial.print("date_str:");
        Serial.println(date_str);
      }
      if (gps.time.isValid()){
        time_str = "";
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();
        minute = (minute + 30);
        if (minute > 59){
          minute = minute - 60;
          hour = hour + 1;
        }
        hour = (hour + 5) ;
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
        Serial.print("time_str:");
        Serial.println(time_str);
      }
      if (gps.speed.isValid()) {
        float speed_kmph = gps.speed.kmph();
        Serial.print("Speed (km/h): ");
        Serial.println(speed_kmph, 6);
      }
    }
  delay(10);
}
