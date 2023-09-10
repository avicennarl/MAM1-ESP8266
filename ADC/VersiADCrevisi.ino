#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "ESP8266-Hotspot";
const char *password = "password123";

const int buzzerPin = D2;
const int analogInputPin = A0;

int noteFrequencies[] = {262, 294, 330, 349, 392, 440, 494, 523};
int balonkuAdaLima[] = {0, 0, 4, 4, 5, 5, 4, 3, 3, 2, 2, 1, 1, 0};
int balonkuAdaLimaLength = 14;
int currentNoteIndex = 0;
bool isPlaying = false;

ESP8266WebServer server(80);
bool potentiometerActive = true;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(analogInputPin, INPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/setAnalogValue", HTTP_POST, handleSetAnalogValue);
  server.on("/playSound", HTTP_GET, handlePlaySound);
  server.on("/playBalonku", HTTP_GET, handlePlayBalonku);
  server.on("/stopSound", HTTP_GET, handleStopSound);
  server.on("/activatePotentiometer", HTTP_GET, handleActivatePotentiometer);
  server.on("/deactivatePotentiometer", HTTP_GET, handleDeactivatePotentiometer);
  server.begin();
}

void loop() {
  server.handleClient();
  if (potentiometerActive) {
    int sensorValue = analogRead(analogInputPin);
    int noteIndex = map(sensorValue, 0, 1023, 0, 7);
    int frequency = noteFrequencies[noteIndex];
    if (!isPlaying) {
      tone(buzzerPin, frequency);
    }
    delay(100);
    if (!isPlaying) {
      noTone(buzzerPin);
    }
  }
  if (isPlaying) {
    int noteIndex = balonkuAdaLima[currentNoteIndex];
    int frequency = noteFrequencies[noteIndex];
    tone(buzzerPin, frequency);
    delay(500);
    noTone(buzzerPin);
    delay(100);
    currentNoteIndex = (currentNoteIndex + 1) % balonkuAdaLimaLength;
  }
}

void handleRoot() {
  String html = "<html><head>;
  
  
  </body></html>";
  server.send(200, "text/html", html);
}

void handleSetAnalogValue() {
  if (server.hasArg("analogValue")) {
    int newValue = server.arg("analogValue").toInt();
    analogWrite(analogInputPin, newValue);
    server.send(200, "text/plain", "Nilai analog telah diatur: " + String(newValue));
  } else {
    server.send(400, "text/plain", "Parameter analogValue tidak ditemukan");
  }
}

void handlePlaySound() {
  if (server.hasArg("note")) {
    int noteIndex = server.arg("note").toInt();
    if (noteIndex >= 0 && noteIndex < 8) {
      int frequency = noteFrequencies[noteIndex];
      tone(buzzerPin, frequency);
      delay(500);
      noTone(buzzerPin);
    }
  }
  handleRoot();
}

void handlePlayBalonku() {
  isPlaying = true;
  currentNoteIndex = 0;
  handleRoot();
}

void handleStopSound() {
  isPlaying = false;
  noTone(buzzerPin);
  handleRoot();
}

void handleActivatePotentiometer() {
  potentiometerActive = true;
  handleRoot();
}

void handleDeactivatePotentiometer() {
  potentiometerActive = false;
  handleRoot();
}
