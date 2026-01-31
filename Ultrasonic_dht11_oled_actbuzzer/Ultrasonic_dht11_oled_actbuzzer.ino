#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
DHT dht(2, DHT11);

const int trigPin = 9;
const int echoPin = 10;
const int buttonPin = 3;
const int ledPin = 13;
const int buzzerPin = 11; 

bool systemActive = false;
float readings[10];
int readIndex = 0;
float total = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT); // For active buzzer
  pinMode(buttonPin, INPUT_PULLUP); 

  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  // Initialize smoothing array
  for (int i = 0; i < 10; i++) readings[i] = 0;
  display.clearDisplay();
}

void loop() {
  // 1. Toggle Button
  if (digitalRead(buttonPin) == LOW) {
    systemActive = !systemActive;
    delay(300); // Debounce
  }

  // 2. Read DHT11 Data
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // 3. Sensor Math (Distance)
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 26000);
  float rawDist = (duration * 0.0343) / 2;

  // Smoothing distance
  if (rawDist > 0) {
    total = total - readings[readIndex];
    readings[readIndex] = rawDist;
    total = total + readings[readIndex];
    readIndex = (readIndex + 1) % 10;
  }
  float avgDist = total / 10;

  // 4. Alert Logic (Active Buzzer + LED)
  if (systemActive && avgDist > 1.0 && avgDist < 10.0) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH); // Turn Active Buzzer ON
    delay(50);                     // Short beep
    digitalWrite(buzzerPin, LOW);  // Turn Active Buzzer OFF
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }

  // 5. OLED Output (Improved Layout)
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  // --- Header / Status (Top of Yellow Section) ---
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("STATUS: ");
  display.print(systemActive ? "ALARM ON" : "ALARM OFF");
  display.drawLine(0, 11, 128, 11, SSD1306_WHITE); // Horizontal line

  // --- Environment Row (Bottom of Yellow Section) ---
  display.setCursor(0, 16); 
  display.print("TEMP: "); display.print(t, 1); display.print("C");
  
  display.setCursor(70, 16); // Move cursor to the middle
  display.print("HUM: "); display.print((int)h); display.print("%");

  // --- Distance Section (Main Blue Area) ---
  display.setCursor(0, 35);
  display.print("DISTANCE:");
  
  display.setCursor(20, 48);
  display.setTextSize(2); // Large font for the most important number
  if (avgDist < 2 || avgDist > 400) {
    display.print("---");
  } else {
    display.print(avgDist, 1);
    display.print("cm");
  }
  
  display.display();
}