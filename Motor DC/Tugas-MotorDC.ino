#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "NamaHotspot";
const char* password = "KataSandiHotspot";

const int motorPin1 = D1;
const int motorPin2 = D2;
const int enablePin = D3;

ESP8266WebServer server(80);

int motorSpeed = 0;

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.print("Hotspot IP Address: ");
  Serial.println(WiFi.softAPIP());
  server.on("/", HTTP_GET, handleRoot);
  server.on("/control", HTTP_POST, handleControl);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>Kendalikan Motor DC</title>";
  html += "<style>body{font-family:Arial,sans-serif;text-align:center;}.button{display:inline-block;padding:10px 20px;font-size:18px;background-color:#3498db;color:#fff;border:none;cursor:pointer;margin:10px;}.slider{width:80%;}</style>";
  html += "</head><body><h1>Kendalikan Motor DC</h1>";

  // Bagian slider kecepatan
  html += "<label for='speedSlider'>Kecepatan:</label>";
  html += "<input type='range' min='0' max='255' value='0' class='slider' id='speedSlider'><br>";

  // Tombol-tombol kendali
  html += "<button class='button' onclick='forward()'>Maju</button>";
  html += "<button class='button' onclick='reverse()'>Mundur</button>";
  html += "<button class='button' onclick='stopMotor()'>Berhenti</button>";

  // Script JavaScript
  html += "<script>";
  html += "function sendCommand(command, speed){var xhr=new XMLHttpRequest();xhr.open('POST','/control',!0);xhr.setRequestHeader('Content-Type','application/x-www-form-urlencoded');xhr.send('command='+command+'&speed='+speed);}";
  html += "function forward(){var speed=document.getElementById('speedSlider').value;sendCommand('forward', speed);}";
  html += "function reverse(){var speed=document.getElementById('speedSlider').value;sendCommand('reverse', speed);}";
  html += "function stopMotor(){sendCommand('stop', 0);}";
  html += "</script>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}


void handleControl() {
  String command = server.arg("command");
  int speed = server.arg("speed").toInt();

  analogWrite(enablePin, speed);

  if (command == "forward") {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
  } else if (command == "reverse") {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
  } else if (command == "stop") {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }
  
  server.send(200, "text/plain", "OK");
}
