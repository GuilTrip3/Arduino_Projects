#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -------- OLED --------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// -------- WiFi --------
const char* ssid = "VM2540409";
const char* password = "ve4fHh4uuVyrnjxs";

// -------- API --------
const char* apiURL =
  "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin,ethereum&vs_currencies=eur";

// -------- Helper: format price --------
String formatPrice(long price) {
  if (price >= 1000) {
    float k = price / 1000.0;
    return String(k, 1) + "k";
  }
  return String(price);
}

void setup() {
  Serial.begin(115200);

  Wire.begin(4, 5); // SDA, SCL

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);

  // WiFi connect screen
  display.setTextSize(1);
  display.setCursor(0, 24);
  display.println("Connecting WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }

  display.clearDisplay();
  display.setCursor(20, 24);
  display.println("Connected");
  display.display();
  delay(800);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient https;
    https.begin(client, apiURL);

    int httpCode = https.GET();
    if (httpCode == HTTP_CODE_OK) {

      StaticJsonDocument<256> doc;
      deserializeJson(doc, https.getString());

      long btc = doc["bitcoin"]["eur"];
      long eth = doc["ethereum"]["eur"];

      // ---------- DRAW UI ----------
      display.clearDisplay();

      // Header
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.print("CRYPTO");
      display.setCursor(88, 0);
      display.print("EUR");

      display.drawLine(0, 10, 127, 10, WHITE);

      // Labels
      display.setCursor(8, 14);
      display.print("BTC");
      display.setCursor(78, 14);
      display.print("ETH");

      // Prices
      display.setTextSize(2);
       display.setCursor(0, 30);
      display.print(formatPrice(btc));

      display.setCursor(64, 30);
      display.print(formatPrice(eth));

      display.display();
    }

    https.end();
  }

  delay(20000); // refresh every 20s
}

