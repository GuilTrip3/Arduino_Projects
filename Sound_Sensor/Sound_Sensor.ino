int redLED = 13;
int yellowLED = 12;
int greenLED = 11;

int soundPin = 2;

bool running = false;
bool lastSoundState = HIGH;

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  pinMode(soundPin, INPUT);
}

void loop() {
  bool soundState = digitalRead(soundPin);

  // Detect clap (HIGH → LOW)
  if (lastSoundState == HIGH && soundState == LOW) {
    running = !running;   // toggle LEDs
    delay(300);           // debounce / ignore echoes
  }

  lastSoundState = soundState;

  if (running) {
    digitalWrite(greenLED, HIGH);
    delay(500);
    digitalWrite(greenLED, LOW);

    digitalWrite(redLED, HIGH);
    delay(500);
    digitalWrite(redLED, LOW);

    digitalWrite(yellowLED, HIGH);
    delay(500);
    digitalWrite(yellowLED, LOW);
  } else {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
  }
}
