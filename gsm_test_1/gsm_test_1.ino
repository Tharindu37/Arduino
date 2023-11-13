#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h> //https://github.com/vshymanskyy/TinyGSM
#include <ArduinoHttpClient.h> //https://github.com/arduino-libraries/ArduinoHttpClient

#include <SoftwareSerial.h>
 

SoftwareSerial sim800(4, 5);
 
 
const char FIREBASE_HOST[]  = "htest-e2db4-default-rtdb.firebaseio.com/";
const String FIREBASE_AUTH  = "Rf1tfDahq1g5hzmrRd9OGfbOO4WXFIC8Q2P2XFbm";
const String FIREBASE_PATH  = "/";
const int SSL_PORT          = 443;
 
char apn[]  = "hutch3g";//airtel ->"airtelgprs.com"
char user[] = "";
char pass[] = "";
 
 
TinyGsm modem(sim800);
 
TinyGsmClientSecure gsm_client_secure_modem(modem, 0);
HttpClient http_client = HttpClient(gsm_client_secure_modem, FIREBASE_HOST, SSL_PORT);
 
unsigned long previousMillis = 0;
 
 
void setup()
{
  Serial.begin(115200);
  Serial.println("device serial initialize");
 
  sim800.begin(9600);
  Serial.println("SIM800L serial initialize");
 
  Serial.println("Initializing modem...");
  modem.restart();
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);
 
  http_client.setHttpResponseTimeout(10 * 1000); //^0 secs timeout
  Serial.print(F("Connecting to "));
  Serial.print(apn);
}
 
void loop()
{
 
  
  if (!modem.gprsConnect(apn, user, pass))
  {
    Serial.print(F("Connecting to "));
    Serial.print(apn);
    Serial.println(" fail");
    delay(1000);
    return;
  }
  else
  {
    Serial.println("GPRS Connected");
    delay(1000);
    while(1);
  }
}
