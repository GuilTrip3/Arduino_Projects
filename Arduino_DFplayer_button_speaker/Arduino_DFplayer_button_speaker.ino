const int buttonPin = 2;
const int dfpIO1 = 3;  // Next track
const int dfpIO2 = 4;  // Volume down

bool lastState = HIGH;
unsigned long pressStart = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(dfpIO1, OUTPUT);
  pinMode(dfpIO2, OUTPUT);

  digitalWrite(dfpIO1, HIGH);
  digitalWrite(dfpIO2, HIGH);
}

void loop() {
  bool currentState = digitalRead(buttonPin);

  // Button pressed
  if (lastState == HIGH && currentState == LOW) {
    pressStart = millis();
  }

  // Button released
  if (lastState == LOW && currentState == HIGH) {
    unsigned long pressDuration = millis() - pressStart;

    if (pressDuration < 800) {
      // SHORT PRESS → Next track
      triggerDFPlayer(dfpIO1, 200);
    } else {
      // LONG PRESS → Volume DOWN
      triggerDFPlayer(dfpIO2, 1000);
    }
  }

  lastState = currentState;
}

void triggerDFPlayer(int pin, int durationMs) {
  digitalWrite(pin, LOW);
  delay(durationMs);
  digitalWrite(pin, HIGH);
}
