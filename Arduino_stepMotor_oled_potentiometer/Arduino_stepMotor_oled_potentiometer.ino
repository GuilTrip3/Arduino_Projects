#include <Stepper.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int stepsPerRevolution = 2048; // 28BYJ-48
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

int lastPosition = 0;

void setup() {
  myStepper.setSpeed(10);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  int potValue = analogRead(A0);
  int targetPosition = map(potValue, 0, 1023, 0, stepsPerRevolution);

  int stepDifference = targetPosition - lastPosition;
  myStepper.step(stepDifference);
  lastPosition = targetPosition;

  float angleDeg = (lastPosition * 360.0) / stepsPerRevolution;
  float angleRad = radians(angleDeg - 90); 
  // -90 so 0° points UP like a compass

  drawCompass(angleDeg, angleRad);

  delay(30);
}

void drawCompass(float angleDeg, float angleRad) {
  display.clearDisplay();

  int cx = 64;   // center X
  int cy = 32;   // center Y
  int r  = 25;   // radius

  // Outer circle
  display.drawCircle(cx, cy, r, SSD1306_WHITE);

  // Cardinal points
  display.setTextSize(1);
  display.setCursor(cx - 3, cy - r - 8); display.print("N");
  display.setCursor(cx - 3, cy + r + 2); display.print("S");
  display.setCursor(cx - r - 8, cy - 3); display.print("W");
  display.setCursor(cx + r + 2, cy - 3); display.print("E");

  // Needle
  int needleX = cx + r * cos(angleRad);
  int needleY = cy + r * sin(angleRad);
  display.drawLine(cx, cy, needleX, needleY, SSD1306_WHITE);
  display.fillCircle(cx, cy, 2, SSD1306_WHITE);

  // Angle text
  display.setCursor(0, 0);
  display.print((int)angleDeg);
  display.print((char)247);

  display.display();
}
