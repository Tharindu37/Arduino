#include <SoftwareSerial.h>
//
//String Arsp, Grsp;
//SoftwareSerial gsm(4, 2); // RX, TX
//
//void setup() {
//  // put your setup code here, to run once:
//
//  Serial.begin(9600);
//  Serial.println("Testing GSM SIM800L");
//  gsm.begin(4800);
//
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  if(gsm.available())
//  {
//    Grsp = gsm.readString();
//    Serial.println(Grsp);
//  }
//
//  if(Serial.available())
//  {
//    Arsp = Serial.readString();
//    gsm.println(Arsp);
//  }
//
//}

String Arsp, Grsp;
HardwareSerial gsm(1);  // Use Serial1 on NodeMCU (RX = D7, TX = D8)

void setup() {
  Serial.begin(9600);
  Serial.println("Testing GSM SIM800L");

  gsm.begin(9600);  // Initiate GSM module
  delay(1000);      // Wait for GSM module to initialize
}

void loop() {
  if(gsm.available()) {
    Grsp = gsm.readString();
    Serial.println(Grsp);
  }

  if(Serial.available()) {
    Arsp = Serial.readString();
    gsm.println(Arsp);
  }
}
