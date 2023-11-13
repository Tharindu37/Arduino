#include <Custom_GPS.h>

//gps code
//#include <TinyGPS++.h>
//#include <SoftwareSerial.h>

//GPS
//TinyGPSPlus gps;  // The TinyGPS++ object
////SoftwareSerial ss(4, 5); // The serial connection to the GPS device D1,D2
//SoftwareSerial ss(5, 4);
//float latitude , longitude;
//int year , month , date, hour , minute , second;
//String date_str , time_str , lat_str , lng_str;
//int pm;

Custom_GPS custom_gps(5,4);

//firebase code
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "test-e2db4-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Rf1tfDahq1g5hzmrRd9OGfbOO4WXFIC8Q2P2XFbm"

//I2C display
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int SCL_pin = 14; //Pin D5
int SDA_pin = 12; //Pin D6

//web server
#include <ESP8266WebServer.h>

const char* ssid = "Bus traking system";
const char* password = "";

IPAddress local_ip(192, 168, 1, 1);
//IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);
String userId = "dd";

//auto connect
#include <DNSServer.h>
//#include <ESP8266WebServer.h>
#include <WiFiManager.h>

void setup(){
  Serial.begin(115200);
//  ss.begin(9600);
  Serial.println();

  //I2C Display
  pinMode(SCL_pin, OUTPUT);
  pinMode(SDA_pin, OUTPUT);
  digitalWrite(SDA_pin,LOW);
  digitalWrite(SCL_pin, LOW);

  Wire.begin(SDA_pin ,SCL_pin);
  lcd.init();// initialize the lcd 
  displayMessage("Setup Your Device.");

//autoConnect wifi
WiFiManager wifiManager;
wifiManager.autoConnect("AutoConnectAP");
Serial.println("connected...yeey :)");
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // web server
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", HTTP_GET, handle_OnConnect);
  server.on("/get", HTTP_GET, handle_get);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  // web server end
  
}
void loop(){
  server.handleClient();
  if(userId!=""){

    custom_gps.update();

    Serial.println("Latitude: " + String(custom_gps.getLatitude(), 6));
    Serial.println("Longitude: " + String(custom_gps.getLongitude(), 6));
    Serial.println("Date: " + custom_gps.getDate());
    Serial.println("Time: " + custom_gps.getTime());
    Serial.println("Speed (km/h): " + String(custom_gps.getSpeed(), 6));
  
    int isStop=Firebase.getBool(userId+"/isStop");
    Serial.println(isStop);
    if(isStop==1){
        displayMessage("Stop");
    }else{
      displayMessage("GO");
    }
    Serial.println();
    if (Firebase.failed()) {
            Serial.print("value /value failed:");
            Serial.println(Firebase.error());  
            delay(1000);
            return;
        }
//  delay(10);
  }
}

void displayMessage(String message){
  // Print a message to the LCD.
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Bus Traking.");
  lcd.setCursor(0,1);
  lcd.print(message);
}

//web server
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

void handle_get() {
  if (server.hasArg("userId")) {
    userId = server.arg("userId");
    Serial.println("User ID: " + userId);
  }
  server.send(200, "text/html", "User ID received: " + userId +
                                   "<br><a href=\"/\">Return to Home Page</a>");
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML() {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>User ID Input</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += "form {margin-bottom: 20px;} input {font-size: 16px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP8266 Web Server</h1>\n";
  ptr += "<h3>Enter your User ID</h3>\n";

  // Add userId input field
  ptr += "<form action=\"/get\">\n";
  ptr += "  User ID: <input type=\"text\" name=\"userId\">\n";
  ptr += "  <input type=\"submit\" value=\"Submit\">\n";
  ptr += "</form>\n";

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
