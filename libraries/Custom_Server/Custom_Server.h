#ifndef Custom_Server_h
#define Custom_Server_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class Custom_Server
{
private:
    String _userId;
    String _ssid;
    String _password;
    String SendHTML();
    void handle_NotFound();
    void handle_get();
    void handle_OnConnect();

public:
    Custom_Server(String ssid, String password);
    String getUserId();
    void loop();
};

#endif