#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h> // Include LittleFS library

#ifndef STASSID
#define STASSID "MojaTV_Full_511570"
#define STAPSK  "GFAQSMVIFWGFAQSMVIFW"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const char* authUser = "admin"; // Embedded username
const char* authPass = "12345"; // Embedded password
const int relayPin = 0;

// Static IP Configuration
IPAddress local_IP(192, 168, 1, 150); 
IPAddress gateway(192, 168, 1, 1);    
IPAddress subnet(255, 255, 255, 0);   

ESP8266WebServer server(80);

void handleGatePress() {
  Serial.println("Gate button pressed");
  digitalWrite(relayPin, HIGH);
  delay(750);
  digitalWrite(relayPin, LOW);
  server.send(200, "text/plain", "Gate Pressed");
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

void handleRoot() {
  if (!server.authenticate(authUser, authPass)) {
    server.requestAuthentication();
    return;
  }
  File file = LittleFS.open("/index.html", "r"); // Use LittleFS instead of SPIFFS
  if (file) {
    server.streamFile(file, "text/html");
    file.close();
  } else {
    server.send(500, "text/plain", "File not found");
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relayPin, OUTPUT);
  
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(relayPin, LOW);

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  // Static IP Configuration
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);
  Serial.println("Attempting to connect to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("");
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }

  // Serve static files
  server.serveStatic("/style.css", LittleFS, "/style.css");

  // Endpoints
  server.on("/", handleRoot);
  server.on("/gate/press", handleGatePress);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
