#define IR_PIN 2

#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

int colorIndex = 0;
bool lastState = HIGH;

void setup() {
  pinMode(IR_PIN, INPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  allOff();
}

void loop() {
  bool currentState = digitalRead(IR_PIN);

  // Detect hand wave (HIGH → LOW)
  if (lastState == HIGH && currentState == LOW) {
    nextColor();
    delay(300); // debounce
  }

  lastState = currentState;
}

void nextColor() {
  colorIndex = (colorIndex + 1) % 8;

  switch (colorIndex) {
    case 0: setColor(0, 0, 0); break;       // OFF
    case 1: setColor(255, 0, 0); break;     // RED
    case 2: setColor(0, 255, 0); break;     // GREEN
    case 3: setColor(0, 0, 255); break;     // BLUE
    case 4: setColor(255, 255, 0); break;   // YELLOW
    case 5: setColor(0, 255, 255); break;   // CYAN
    case 6: setColor(255, 0, 255); break;   // MAGENTA
    case 7: setColor(255, 255, 255); break; // WHITE
  }
}

void setColor(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void allOff() {
  setColor(0, 0, 0);
}
