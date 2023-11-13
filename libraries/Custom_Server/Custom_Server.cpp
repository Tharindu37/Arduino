#include "Custom_Server.h"

Custom_Server::Custom_Server(String ssid, String password)
{
    IPAddress local_ip(192, 168, 1, 1);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);

    ESP8266WebServer server(80);

    _ssid = ssid;
    _password = password;

    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);

    server.on("/", HTTP_GET, handle_OnConnect);
    server.on("/get", HTTP_GET, handle_get);
    server.onNotFound(handle_NotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void Custom_Server::handle_OnConnect()
{
    server.send(200, "text/html", SendHTML());
}

void Custom_Server::handle_get()
{
    if (server.hasArg("userId"))
    {
        _userId = server.arg("userId");
        Serial.println("User ID: " + _userId);
    }
    server.send(200, "text/html", "User ID received: " + userId + "<br><a href=\"/\">Return to Home Page</a>");
}

void Custom_Server::handle_NotFound()
{
    server.send(404, "text/plain", "Not found");
}

String Custom_Server::SendHTML()
{
    String ptr = "<!DOCTYPE html> <html>\n";
    ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr += "<title>User ID Input</title>\n";
    ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    ptr += "form {margin-bottom: 20px;} input {font-size: 16px;}\n";
    ptr += "</style>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<h1>Real-Time Bus Traking System</h1>\n";
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

String Custom_Server::getUserId()
{
    return _userId;
}

void Custom_Server::loop()
{
    server.handleClient();
}