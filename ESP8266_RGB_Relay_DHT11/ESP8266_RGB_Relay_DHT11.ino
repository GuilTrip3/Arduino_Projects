#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

/* ===== WIFI ===== */
const char* ssid = "VM2540409";
const char* password = "ve4fHh4uuVyrnjxs";

/* ===== WEB SERVER ===== */
ESP8266WebServer server(80);

/* ===== SECURITY ===== */
const String PIN_CODE = "1234";

/* ===== RGB ===== */
#define RED_PIN   14
#define GREEN_PIN 12
#define BLUE_PIN  13

/* ===== DHT ===== */
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

/* ===== RELAY ===== */
#define RELAY_PIN 5
bool relayState = false;

/* ===== VARIABLES ===== */
int redVal = 0, greenVal = 0, blueVal = 0;
int humidityThreshold = 60;

float temperature = 0;
float humidity = 0;

/* ===== FUNCTIONS ===== */

bool checkPin() {
  return server.hasArg("pin") && server.arg("pin") == PIN_CODE;
}

void setColor() {
  analogWrite(RED_PIN, redVal);
  analogWrite(GREEN_PIN, greenVal);
  analogWrite(BLUE_PIN, blueVal);
}

void updateRelay() {
  relayState = humidity > humidityThreshold;
  digitalWrite(RELAY_PIN, relayState ? LOW : HIGH); // ACTIVE LOW
}

void readDHT() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
}

void handleRoot() {
  readDHT();
  updateRelay();

  String html = "<html><head><meta name='viewport' content='width=device-width'>";
  html += "<title>ESP Control</title></head><body>";
  html += "<h2>ESP8266 RGB + DHT + Relay</h2>";

  html += "<p><b>Temperature:</b> " + String(temperature) + " °C</p>";
  html += "<p><b>Humidity:</b> " + String(humidity) + " %</p>";
  html += "<p><b>Relay:</b> " + String(relayState ? "ON" : "OFF") + "</p>";

  html += "<form action='/set'>PIN:<br>";
  html += "<input type='password' name='pin'><br><br>";

  html += "Red:<br><input type='range' name='r' min='0' max='1023' value='" + String(redVal) + "'><br>";
  html += "Green:<br><input type='range' name='g' min='0' max='1023' value='" + String(greenVal) + "'><br>";
  html += "Blue:<br><input type='range' name='b' min='0' max='1023' value='" + String(blueVal) + "'><br><br>";

  html += "Humidity Threshold (%):<br>";
  html += "<input type='number' name='h' value='" + String(humidityThreshold) + "'><br><br>";

  html += "<input type='submit' value='Apply'></form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleSet() {
  if (!checkPin()) {
    server.send(403, "text/plain", "Invalid PIN");
    return;
  }

  if (server.hasArg("r")) redVal = server.arg("r").toInt();
  if (server.hasArg("g")) greenVal = server.arg("g").toInt();
  if (server.hasArg("b")) blueVal = server.arg("b").toInt();
  if (server.hasArg("h")) humidityThreshold = server.arg("h").toInt();

  setColor();
  updateRelay();

  server.sendHeader("Location", "/");
  server.send(303);
}

/* ===== SETUP ===== */
void setup() {
  Serial.begin(115200);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH); // relay OFF
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
}

/* ===== LOOP ===== */
void loop() {
  server.handleClient();
}
