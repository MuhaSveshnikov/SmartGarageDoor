#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

#ifndef STASSID
#define STASSID "MojaTV_Full_511570"
#define STAPSK  "GFAQSMVIFWGFAQSMVIFW"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const int relayPin = 0;

// Allowed email for the authenticated user
const String allowedUserEmail = "muhamed1911.mm@gmail.com";

IPAddress local_IP(192, 168, 1, 150);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(8080);

void handleGatePress() {
  // Get user email from the headers
  String userEmail = server.arg("email");

  if (userEmail == allowedUserEmail) {
    Serial.println("Authorized user: Gate button pressed");
    digitalWrite(relayPin, HIGH);
    delay(750);
    digitalWrite(relayPin, LOW);
    server.send(200, "application/json", "{\"status\": \"success\"}");
  } else {
    Serial.println("Unauthorized user attempted to access the gate");
    server.send(403, "application/json", "{\"error\": \"Unauthorized\"}");
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

void handleRoot() {
  File file = LittleFS.open("/index.html", "r");
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
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }

  server.serveStatic("/style.css", LittleFS, "/style.css");
  server.serveStatic("/script.js", LittleFS, "/script.js");

  server.on("/", handleRoot);
  server.on("/gate/press", handleGatePress);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
