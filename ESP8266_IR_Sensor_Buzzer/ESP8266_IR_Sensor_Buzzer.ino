#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi credentials
const char* ssid = "VM2540409";
const char* password = "ve4fHh4uuVyrnjxs";

// Web server
ESP8266WebServer server(80);

// Pins
#define IR_PIN 4      // IR sensor (LOW = object detected)
#define BUZZER_PIN 14  // Passive buzzer

// State
bool objectDetected = false;

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Server routes
  server.on("/", handleRoot);
  server.on("/clear", handleClear);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();

  // Check IR sensor
  objectDetected = (digitalRead(IR_PIN) == LOW);

  // If object is detected, buzzer ON, else OFF
  if (objectDetected) {
    digitalWrite(BUZZER_PIN, HIGH); // Keep buzzing
    Serial.println("Object detected!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(50); // Small delay to avoid rapid looping
}

// Web page showing status
void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<title>IR Object Detector</title>";
  html += "<style>body{font-family:Arial;text-align:center;padding:20px;background:#222;color:#0f0;}";
  html += ".status{font-size:3em;margin:20px;padding:20px;border-radius:10px;}";
  html += ".detected{background:#f44;}.clear{background:#4f4;}</style></head><body>";

  html += "<h1>🤖 IR Object Detector</h1>";
  html += "<div class='status " + String(objectDetected ? "detected" : "clear") + "'>";
  html += objectDetected ? "🚨 OBJECT DETECTED 🚨" : "👌 No Object";
  html += "</div>";

  if (objectDetected) {
    html += "<p><a href='/clear' style='color:#ff0;font-size:1.2em;padding:10px 20px;background:#440;border-radius:5px;text-decoration:none;'>Clear Alert</a></p>";
  }

  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Clear detection manually
void handleClear() {
  objectDetected = false;
  digitalWrite(BUZZER_PIN, LOW);
  server.send(200, "text/html", "<h2>Alert cleared</h2><a href='/'>Back</a>");
}
