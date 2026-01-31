// LED pin definitions
int redLED    = 13;
int yellowLED = 12;
int greenLED  = 11;

// Button pin
int buttonPin = 3;

// Buzzer pin (Active buzzer)
int buzzerPin = 6;

// Potentiometer pin
int potPin = A0;

// State variables
bool running = false;
bool lastButtonState = HIGH;

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

// Short beep feedback
void beepFeedback() {
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  // Read button
  bool buttonState = digitalRead(buttonPin);

  // Read potentiometer and map to delay (100ms – 1000ms)
  int potValue = analogRead(potPin);
  int ledDelay = map(potValue, 0, 1023, 100, 1000);

  // Detect button press
  if (lastButtonState == HIGH && buttonState == LOW) {
    beepFeedback();
    running = !running;
    delay(200); // debounce
  }
  lastButtonState = buttonState;

  if (running) {
    digitalWrite(greenLED, HIGH);
    delay(ledDelay);
    digitalWrite(greenLED, LOW);

    digitalWrite(redLED, HIGH);
    delay(ledDelay);
    digitalWrite(redLED, LOW);

    digitalWrite(yellowLED, HIGH);
    delay(ledDelay);
    digitalWrite(yellowLED, LOW);
  } else {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
  }
}
