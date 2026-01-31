#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

int currentR = 0;
int currentG = 0;
int currentB = 0;

String lastStatus = "";

void setup() {
  Serial.begin(9600);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    while (true);
  }

  display.setTextColor(SSD1306_WHITE);

  fadeToColor(255, 255, 255);
  showMessage("Starting...");
  delay(2000);
}

void loop() {
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();

  if (isnan(humidity) || isnan(temp)) {
    if (lastStatus != "Error") {
      fadeToColor(255, 255, 255);
      showMessage("Sensor Error");
      lastStatus = "Error";
    }
    delay(2000);
    return;
  }

  String status;
  int targetR, targetG, targetB;

  if (humidity >= 60) {
    status = "Humid";
    targetR = 255; targetG = 0; targetB = 255;
  }
  else if (temp < 20) {
    status = "Cold";
    targetR = 0; targetG = 0; targetB = 255;
  }
  else if (temp < 26) {
    status = "Comfort";
    targetR = 0; targetG = 255; targetB = 0;
  }
  else {
    status = "Hot";
    targetR = 255; targetG = 0; targetB = 0;
  }

  if (status != lastStatus) {
    fadeToColor(targetR, targetG, targetB);
    showStatus(temp, humidity, status);
    lastStatus = status;
  }

  delay(2000);
}

void fadeToColor(int targetR, int targetG, int targetB) {
  const int steps = 50;

  for (int i = 1; i <= steps; i++) {
    int r = currentR + (targetR - currentR) * i / steps;
    int g = currentG + (targetG - currentG) * i / steps;
    int b = currentB + (targetB - currentB) * i / steps;

    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);
    delay(20);
  }

  currentR = targetR;
  currentG = targetG;
  currentB = targetB;
}

void showStatus(float temp, float humidity, String status) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temp);
  display.println(" C");

  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  display.println();
  display.setTextSize(2);
  display.println(status);

  display.display();
}

void showMessage(String message) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println(message);
  display.display();
}
