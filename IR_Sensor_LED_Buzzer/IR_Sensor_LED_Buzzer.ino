int irPin = 2;
int ledPin = 8;
int buzzerPin = 6;

void setup() {
  pinMode(irPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  int irState = digitalRead(irPin);

  if (irState == LOW) {   // Object detected
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000);   // 1 kHz sound
  } else {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);       // Stop sound
  }
}
