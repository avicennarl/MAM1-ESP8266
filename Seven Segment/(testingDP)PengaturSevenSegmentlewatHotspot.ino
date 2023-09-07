#include <ESP8266WiFi.h>

const char* ssid = "MyESP8266Hotspot"; // Nama hotspot WiFi yang akan dipancarkan
const char* password = "Password123";   // Kata sandi hotspot WiFi Anda

// Seven segment pins attached with NodeMCU pins
int a = D0;  // Gpio-0 with a of 7 segment display   
int b = D1;  // Gpio-1 with b of 7 segment display    
int c = D2;  // Gpio-2 with c of 7 segment display  
int d = D3;  // Gpio-3 with d of 7 segment display  
int e = D4;  // Gpio-4 with e of 7 segment display   
int f = D5;  // Gpio-5 with f of 7 segment display  
int g = D6;  // Gpio-6 with g of 7 segment display
int dp = D7; // Gpio-7 with dp (decimal point) of 7 segment display

WiFiServer server(80);

bool dpState = false; // State to toggle DP on/off

void setup() {
  Serial.begin(115200);
  delay(10);

  // Inisialisasi NodeMCU sebagai titik akses WiFi
  WiFi.softAP(ssid, password);

  // IP address dari NodeMCU saat berada dalam mode titik akses
  IPAddress apIP(192, 168, 1, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  // Declaring Seven segment pins as Output
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(dp, OUTPUT); // Set DP pin as OUTPUT

  // Set semua segmen Seven Segment menjadi non-aktif
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(dp, HIGH); // Set DP pin to off

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print IP address hotspot di serial monitor
  Serial.print("Hotspot IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Print SSID hotspot di serial monitor
  Serial.print("Hotspot SSID: ");
  Serial.println(ssid);
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');

  int value = 0;

  if (request.indexOf("/Req=1") != -1) {
    // Displaying 1
    setSevenSegment(1);
    value = 1;
  } else if (request.indexOf("/Req=2") != -1) {
    // Displaying 2
    setSevenSegment(2);
    value = 2;
  } else if (request.indexOf("/Req=3") != -1) {
  // Displaying 3
  setSevenSegment(3);
  value = 3;
} else if (request.indexOf("/Req=4") != -1) {
  // Displaying 4
  setSevenSegment(4);
  value = 4;
} else if (request.indexOf("/Req=5") != -1) {
  // Displaying 5
  setSevenSegment(5);
  value = 5;
} else if (request.indexOf("/Req=6") != -1) {
  // Displaying 6
  setSevenSegment(6);
  value = 6;
} else if (request.indexOf("/Req=7") != -1) {
  // Displaying 7
  setSevenSegment(7);
  value = 7;
} else if (request.indexOf("/Req=8") != -1) {
  // Displaying 8
  setSevenSegment(8);
  value = 8;
} else if (request.indexOf("/Req=9") != -1) {
  // Displaying 9
  setSevenSegment(9);
  value = 9;
}

  // Check for DP toggle request
  if (request.indexOf("/DPToggle") != -1) {
    // Toggle DP state
    dpState = !dpState;
    digitalWrite(dp, dpState ? LOW : HIGH); // Set DP pin based on dpState
  }

  // Send the HTTP response
  sendHttpResponse(client, value);
}

void setSevenSegment(int number) {
  // Set semua segmen menjadi non-aktif (HIGH)
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);

  switch (number) {
    case 1:
      // Aktifkan segmen-segmen yang diperlukan untuk menampilkan 1
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      break;
    case 2:
      // Aktifkan segmen-segmen yang diperlukan untuk menampilkan 2
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(g, LOW);
      digitalWrite(e, LOW);
      digitalWrite(d, LOW);
      break;
    case 3:
      // Aktifkan segmen-segmen yang diperlukan untuk menampilkan 3
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(g, LOW);
      break;
    case 4:
      // Aktifkan segmen-segmen yang diperlukan untuk menampilkan 4
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 5:
      // Aktifkan segmen-segmen yang diperlukan untuk menampilkan 5
      digitalWrite(a, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 6:
      // Aktifkan segmen-segmen yang diperlukan untuk menampilkan 6
      digitalWrite(a, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 7:
      // Aktifkan segmen-segmen yang diperlukan untuk menampilkan 7
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      break;
    case 8:
      // Aktifkan segmen-segmen yang diperlukan untuk menampilkan 8
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 9:
      // Aktifkan segmen-segmen yang diperlukan untuk menampilkan 9
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
  // Add button to toggle DP
  client.print("<p align=center><a href=\"/DPToggle\"><button>Toggle DP</button></a></p>");
  client.println("</body>");
  client.println("</html>");
  delay(1);
  client.stop();
}
