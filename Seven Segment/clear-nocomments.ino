#include <ESP8266WiFi.h>

const char* ssid = "MyESP8266Hotspot"; 
const char* password = "Password123";   

int a = D0;  
int b = D1; 
int c = D2;  
int d = D3;   
int e = D4;  
int f = D5; 
int g = D6; 
int dp = D7; 

WiFiServer server(80);
bool dpState = false; 

void setup() {
  Serial.begin(115200);
  delay(10);

  WiFi.softAP(ssid, password);

  IPAddress apIP(192, 168, 1, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(dp, OUTPUT); 
  
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(dp, HIGH); 

  
  server.begin();
  Serial.println("Server started");

  
  Serial.print("Hotspot IP Address: ");
  Serial.println(WiFi.softAPIP());

  
  Serial.print("Hotspot SSID: ");
  Serial.println(ssid);
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  String request = client.readStringUntil('\r');
  int value = 0;
  if (request.indexOf("/Req=1") != -1) {
    setSevenSegment(1);
    value = 1;
  } else if (request.indexOf("/Req=2") != -1) {
    setSevenSegment(2);
    value = 2;
  } else if (request.indexOf("/Req=3") != -1) {
  setSevenSegment(3);
  value = 3;
} else if (request.indexOf("/Req=4") != -1) {
  setSevenSegment(4);
  value = 4;
} else if (request.indexOf("/Req=5") != -1) {
  setSevenSegment(5);
  value = 5;
} else if (request.indexOf("/Req=6") != -1) {
  setSevenSegment(6);
  value = 6;
} else if (request.indexOf("/Req=7") != -1) {
  setSevenSegment(7);
  value = 7;
} else if (request.indexOf("/Req=8") != -1) {
  setSevenSegment(8);
  value = 8;
} else if (request.indexOf("/Req=9") != -1) {
  setSevenSegment(9);
  value = 9;
}
  if (request.indexOf("/DPToggle") != -1) {
    dpState = !dpState;
    digitalWrite(dp, dpState ? LOW : HIGH);
  }
  sendHttpResponse(client, value);
}
void setSevenSegment(int number) {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  switch (number) {
    case 1:
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      break;
    case 2:
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(g, LOW);
      digitalWrite(e, LOW);
      digitalWrite(d, LOW);
      break;
    case 3:
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(g, LOW);
      break;
    case 4:
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 5:
      digitalWrite(a, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 6:
      digitalWrite(a, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 7:
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      break;
    case 8:
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 9:
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
  }
}
void sendHttpResponse(WiFiClient client, int value) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("</head>");
  client.println("<body>");
  client.println("<h1 align=center>7 segment display control</h1><br><br>");
  client.print("<p align=center>Currently Displaying = ");
  client.print(value);
  client.println("</p>");
   for (int i = 1; i <= 9; i++) {
    client.print("<p align=center><a href=\"/Req=");
    client.print(i);
    client.print("\"><button>Display-");
    client.print(i);
    client.println("</button></a></p>");
  }
  client.print("<p align=center><a href=\"/DPToggle\"><button>Toggle DP</button></a></p>");
  client.println("</body>");
  client.println("</html>");
  delay(1);
  client.stop();
}
