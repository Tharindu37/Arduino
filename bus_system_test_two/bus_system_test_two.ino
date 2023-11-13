//gps code
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//GPS
TinyGPSPlus gps;  // The TinyGPS++ object
//SoftwareSerial ss(4, 5); // The serial connection to the GPS device D1,D2
SoftwareSerial ss(5, 4);
float latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;

//firebase code
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "test-e2db4-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Rf1tfDahq1g5hzmrRd9OGfbOO4WXFIC8Q2P2XFbm"
#define WIFI_SSID "Tharindu’s iPhone"
#define WIFI_PASSWORD "12345678Tt@"


//I2C display
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int SCL_pin = 14; //Pin D5
int SDA_pin = 12; //Pin D6

void setup(){
  Serial.begin(115200);
  ss.begin(9600);
  Serial.println();

  //connect to wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //I2C Display
  pinMode(SCL_pin, OUTPUT);
  pinMode(SDA_pin, OUTPUT);
  digitalWrite(SDA_pin,LOW);
  digitalWrite(SCL_pin, LOW);

  Wire.begin(SDA_pin ,SCL_pin);
  lcd.init();// initialize the lcd 
  displayMessage();
  
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
        Firebase.setFloat("userId/longitude",longitude);
        if (Firebase.failed()) {
            Serial.print("value /value failed:");
            Serial.println(Firebase.error());  
            delay(1000);
            return;
        }
        Serial.print("latitude:");
        Serial.println(latitude);
        Firebase.setFloat("userId/latitude",latitude);
        if (Firebase.failed()) {
            Serial.print("value /value failed:");
            Serial.println(Firebase.error());  
            delay(1000);
            return;
        }
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
        Firebase.setString("userId/date",date_str);
        if (Firebase.failed()) {
            Serial.print("value /value failed:");
            Serial.println(Firebase.error());  
            delay(1000);
            return;
        }
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
        Firebase.setString("userId/time",time_str);
        if (Firebase.failed()) {
            Serial.print("value /value failed:");
            Serial.println(Firebase.error());  
            delay(1000);
            return;
        }
      }
      if (gps.speed.isValid()) {
        float speed_kmph = gps.speed.kmph();
        Serial.print("Speed (km/h): ");
        Serial.println(speed_kmph, 6);
        Firebase.setFloat("userId/speed",speed_kmph);
        if (Firebase.failed()) {
            Serial.print("value /value failed:");
            Serial.println(Firebase.error());  
            delay(1000);
            return;
        }
      }
    }
  delay(10);
}

void displayMessage(){
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("Tharindu Lakshan");
}
