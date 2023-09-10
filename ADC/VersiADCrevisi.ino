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
  String html = "<html><head>";
  html += "<style>";
  html += "body {";
  html += "  font-family: Arial, sans-serif;";
  html += "  text-align: center;";
  html += "  background-color: #f2f2f2;";
  html += "}";
  html += ".container {";
  html += "  max-width: 600px;";
  html += "  margin: 0 auto;";
  html += "  padding: 20px;";
  html += "  background-color: white;";
  html += "  border-radius: 10px;";
  html += "  box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);";
  html += "}";
  html += ".button-container {";
  html += "  display: grid;";
  html += "  grid-template-columns: repeat(4, 1fr);"; // Ubah menjadi 3 kolom dan 4 baris
  html += "  gap: 20px;";
  html += "  align-items: center;";
  html += "  justify-items: center;";
  html += "}";
  html += ".button {";
  html += "  width: 60px;";
  html += "  height: 60px;";
  html += "  font-size: 20px;";
  html += "  background-color: #007BFF;";
  html += "  color: white;";
  html += "  border: none;";
  html += "  border-radius: 50%;";
  html += "  cursor: pointer;";
  html += "  text-align: center;";
  html += "  line-height: 60px;";
  html += "  text-decoration: none;";
  html += "}";
  html += ".button-off {";
  html += "  background-color: #DC3545;";
  html += "}";
  html += ".title {";
  html += "  font-size: 24px;";
  html += "  margin-bottom: 10px;";
  html += "}";
  html += "</style>";
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<h1 class='title'>ADC NOTE CONTROL</h1>";
  html += "</div>";
  const char *noteNames[] = {"Do", "Re", "Mi", "Fa", "Sol", "La", "Si", "Do"};
  html += "<div class='container button-container'>";
  for (int i = 0; i < 8; i++) {
    html += "<a href='/playSound?note=" + String(i) + "' target='_self' class='button'>" + noteNames[i] + "</a>";
  }
  html += "<a href='/playBalonku' class='button'>Play Balonku</a>";
  html += "<a href='/stopSound' class='button button-off'>Stop Lagu</a>";
  html += "<a href='/deactivatePotentiometer' class='button button-off'>OFF</a>";
  html += "<a href='/activatePotentiometer' class='button'>ON</a>";
  html += "</div>";
  html += "</body></html>";
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
