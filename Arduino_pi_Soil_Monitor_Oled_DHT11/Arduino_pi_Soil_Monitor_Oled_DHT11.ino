#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2
#define DHTTYPE DHT11
#define SOIL_PIN A0
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    for(;;); // Don't proceed, loop forever if OLED fails
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int rawSoil = analogRead(SOIL_PIN); 

  // 1. Calculate Percentage
  int soilPercent = map(rawSoil, 1023, 0, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100); // Ensures it stays between 0-100

  // 2. Determine Condition Text
  String statusText = "";
  if (soilPercent <= 10) {
    statusText = "DRY";
  } else if (soilPercent <= 50) {
    statusText = "DAMP";
  } else {
    statusText = "HYDRATED";
  }

  if (isnan(h) || isnan(t)) return;

  // 3. Send to Pi (CSV Format)
  Serial.print(t);
  Serial.print(",");
  Serial.print(h);
  Serial.print(",");
  Serial.println(rawSoil);

  // 4. Update OLED
  display.clearDisplay();
  
  // Header
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("PLANT STATUS: ");
  display.println(statusText); // Displays Dry, Damp, or Hydrated
  display.drawLine(0, 12, 128, 12, WHITE);

  // Large Moisture Display
  display.setTextSize(3); // Larger font for the percentage
  display.setCursor(10, 25);
  display.print(soilPercent);
  display.print("%");

  // Bottom Stats
  display.setTextSize(1);
  display.setCursor(0, 55);
  display.print("T: "); display.print(t, 1); display.print("C  ");
  display.print("H: "); display.print(h, 0); display.print("%");
  
  display.display();

  delay(2000); 
}
