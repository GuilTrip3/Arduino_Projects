#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

/* ===== WIFI ===== */
const char* ssid = "VM2540409";
const char* password = "ve4fHh4uuVyrnjxs";

/* ===== NTFY ===== */
const char* ntfyHost = "ntfy.sh";
const char* ntfyTopic = "plant_alert_rohit";  // must match phone

ESP8266WebServer server(80);
WiFiClient client;

// GPIO
#define SOIL_PIN 2   // GPIO4

bool soilDry = false;
bool alertSent = false;

void setup() {
  Serial.begin(115200);
  pinMode(SOIL_PIN, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();

  // HIGH = DRY (as confirmed earlier)
  soilDry = (digitalRead(SOIL_PIN) == HIGH);

  if (soilDry && !alertSent) {
    sendNtfy("🚨 Soil is DRY! Time to water your plant 🌱");
    alertSent = true;
  }

  // Reset once soil is wet again
  if (!soilDry) {
    alertSent = false;
  }

  delay(2000);
}

/* ===== SEND NOTIFICATION ===== */
void sendNtfy(String message) {
  if (!client.connect(ntfyHost, 80)) {
    Serial.println("ntfy connection failed");
    return;
  }

  client.print(
    String("POST /") + ntfyTopic + " HTTP/1.1\r\n" +
    "Host: " + ntfyHost + "\r\n" +
    "Content-Type: text/plain\r\n" +
    "Content-Length: " + message.length() + "\r\n\r\n" +
    message
  );

  Serial.println("ntfy alert sent");
}

/* ===== WEB PAGE ===== */
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
  html += soilDry ? "🚨 Soil is DRY<br>Alert sent!" :
                    "💧 Soil is WET<br>All good!";
  html += "</div>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}
