#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Motor pins (L293D)
const int enPin  = 5;  // EN1 (PWM!)
const int in1Pin = 6;
const int in2Pin = 7;

// Button
const int buttonPin = 2;

// RGB LED pins
const int redPin   = 9;
const int greenPin = 10;
const int bluePin  = 11;

// Fan mode
int mode = 0;
bool buttonWasPressed = false;

void setup() {
  // Motor pins
  pinMode(enPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);

  // Button
  pinMode(buttonPin, INPUT_PULLUP);

  // RGB
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)) while(true);
  display.setTextColor(SSD1306_WHITE);

  setFanMode();
}

void loop() {
  bool buttonState = digitalRead(buttonPin) == LOW;

  if(buttonState && !buttonWasPressed){
    mode++;
    if(mode > 3) mode = 0;
    setFanMode();
  }

  buttonWasPressed = buttonState;
  delay(50);
}

void setFanMode(){
  // Turn off RGB first
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);

  switch(mode){
    case 0: // OFF
      analogWrite(enPin,0);
      display.println("FAN OFF");
      break;

    case 1: // LOW - YELLOW
      analogWrite(enPin,150); // PWM
      digitalWrite(redPin,HIGH);
      digitalWrite(greenPin,HIGH);
      display.println("LOW");
      display.setTextSize(1);
      display.println("YELLOW");
      break;

    case 2: // MEDIUM - PURPLE
      analogWrite(enPin,200); // PWM
      digitalWrite(redPin,HIGH);
      digitalWrite(bluePin,HIGH);
      display.println("MED");
      display.setTextSize(1);
      display.println("PURPLE");
      break;

    case 3: // HIGH - RED
      analogWrite(enPin,255); // PWM full speed
      digitalWrite(redPin,HIGH);
      display.println("HIGH");
      display.setTextSize(1);
      display.println("RED");
      break;
  }

  display.display();
}
