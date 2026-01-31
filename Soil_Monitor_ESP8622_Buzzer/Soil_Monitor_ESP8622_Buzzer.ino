#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* ===== WIFI CONFIG ===== */
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
/* ====================== */

ESP8266WebServer server(80);

// GPIO pins
#define SOIL_PIN 2    // GPIO4
#define BUZZER_PIN 5  // GPIO5

bool soilDry = false;

void setup() {
  Serial.begin(9600);

  pinMode(SOIL_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();

  // HIGH = DRY (sensor-specific)
  soilDry = (digitalRead(SOIL_PIN) == HIGH);

  if (soilDry) {
    digitalWrite(BUZZER_PIN, HIGH);  // Buzzer ON when DRY
  } else {
    digitalWrite(BUZZER_PIN, LOW);   // OFF when WET
  }

  delay(500);
}

void handleRoot() {
  String html = "<html><head>";
  html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<style>";
  html += "body{text-align:center;font-family:Arial;background:#111;color:#fff;}";
  html += ".box{font-size:2em;padding:20px;border-radius:12px;}";
  html += ".dry{background:#e74c3c;}";
  html += ".wet{background:#2ecc71;}";
  html += "</style></head><body>";

  html += "<h1>🌱 Plant Monitor</h1>";
  html += "<div class='box " + String(soilDry ? "dry" : "wet") + "'>";
  html += soilDry ? "🚨 Soil is DRY<br>Water now!" :
                    "💧 Soil is WET<br>All good!";
  html += "</div>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}
