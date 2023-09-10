#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

int Pin1 = D1;
int Pin2 = D2;
int Pin3 = D3;
int Pin4 = D4;

int pole1[] = {0, 0, 0, 0, 0, 1, 1, 1, 0};
int pole2[] = {0, 0, 0, 1, 1, 1, 0, 0, 0};
int pole3[] = {0, 1, 1, 1, 0, 0, 0, 0, 0};
int pole4[] = {1, 1, 0, 0, 0, 0, 0, 1, 0};

int poleStep = 0;
int dirStatus = 3;
unsigned long motorEndTime = 0;

const char *ssid = "ESP8266 Hotspot";
const char *password = "password";

ESP8266WebServer server(80);

void handleRoot() {
  String HTML = "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>ESP8266 Stepper Motor Control</title>\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  <style>\
  html, body {\
    width: 100%;\
    height: 100%;\
    margin: 0;\
  }\
  * {\
    box-sizing: border-box;\
  }\
  .colorAll {\
    background-color: #90ee90;\
  }\
  .colorBtn {\
    background-color: #add8e6;\
  }\
  .angleButton, a {\
    font-size: 30px;\
    border: 1px solid #ccc;\
    display: table-caption;\
    padding: 7px 10px;\
    text-decoration: none;\
    cursor: pointer;\
    padding: 5px 6px 7px 10px;\
    display: block;\
  }\
  a {\
    display: block;\
  }\
  .btn {\
    margin: 5px;\
    border: none;\
    display: inline-block;\
    vertical-align: middle;\
    text-align: center;\
    white-space: nowrap;\
  }\
  </style>\
  </head>\
  <body>\
  <h1>ESP8266 Stepper Motor Control</h1>";

  HTML += "<h2>Status Motor</h2>";
  if (dirStatus == 2) {
    HTML += "<h2><span style=\"background-color: #FFFF00\">Motor Berjalan CW</span></h2>";
  } else if (dirStatus == 1) {
    HTML += "<h2><span style=\"background-color: #FFFF00\">Motor Berjalan CCW</span></h2>";
  } else {
    HTML += "<h2><span style=\"background-color: #FFFF00\">Motor Mati</span></h2>";
  }

  HTML += "<form method=\"get\" action=\"/motor\">";
  HTML += "<input type=\"submit\" name=\"dir\" value=\"CW\">";
  HTML += "<input type=\"submit\" name=\"dir\" value=\"CCW\">";
  HTML += "<input type=\"submit\" name=\"dir\" value=\"Mati\">";
  HTML += "</form>";

  HTML += "<h2>Durasi Motor</h2>";
  HTML += "<form method=\"get\" action=\"/setduration\">";
  HTML += "<input type=\"number\" name=\"duration\" value=\"10\" min=\"1\" step=\"1\">";
  HTML += "<input type=\"submit\" value=\"Atur Durasi\">";
  HTML += "</form>";

  HTML += "</body></html>";
  server.send(200, "text/html", HTML);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void setup(void) {
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);
  pinMode(Pin4, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.on("/motor", HTTP_GET, motorControl);
  server.on("/setduration", HTTP_GET, setDuration);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.begin(115200);
  Serial.println("ESP8266 Stepper Motor Control");
}

void loop(void) {
  server.handleClient();

  if (motorEndTime > 0 && millis() >= motorEndTime) {
    dirStatus = 3;
    motorEndTime = 0;
    driveStepper(8);
  }

  if (dirStatus == 1) {
    poleStep++;
    driveStepper(poleStep);
  } else if (dirStatus == 2) {
    poleStep--;
    driveStepper(poleStep);
  } else {
    driveStepper(8);
  }

  if (poleStep > 7) {
    poleStep = 0;
  }
  if (poleStep < 0) {
    poleStep = 7;
  }
  delay(1);
}

void motorControl() {
  String dir = server.arg("dir");

  if (dir == "CW") {
    dirStatus = 2;
  } else if (dir == "CCW") {
    dirStatus = 1;
  } else {
    dirStatus = 3;
  }

  motorEndTime = 0;
  handleRoot();
}

void setDuration() {
  int duration = server.arg("duration").toInt();
  if (duration > 0) {
    motorEndTime = millis() + (duration * 1000);
  }

  handleRoot();
}

void driveStepper(int c) {
  digitalWrite(Pin1, pole1[c]);
  digitalWrite(Pin2, pole2[c]);
  digitalWrite(Pin3, pole3[c]);
  digitalWrite(Pin4, pole4[c]);
}
