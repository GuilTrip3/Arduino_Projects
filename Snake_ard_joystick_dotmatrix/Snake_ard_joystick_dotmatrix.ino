#include "LedControl.h"

// Pin Definitions
const int PIN_X = A0;
const int PIN_Y = A1;
const int PIN_SW = 2;   
const int BUZZER = 8;

// Matrix Pins: DIN (12), CLK (11), CS (10)
LedControl lc = LedControl(12, 11, 10, 1);

// Game Variables
int snakeX[64], snakeY[64];
int snakeLength = 2;
int foodX, foodY;
int dirX = 1, dirY = 0; 
unsigned long lastMoveTime = 0;
int gameSpeed = 300;
bool gameOver = false;
bool gameRunning = false; 

void setup() {
  pinMode(PIN_SW, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);
  initGame();
}

void loop() {
  if (!gameRunning) {
    showStartScreen();
    if (digitalRead(PIN_SW) == LOW) { 
      tone(BUZZER, 1500, 100);
      gameRunning = true;
      lc.clearDisplay(0);
      delay(300); 
    }
    return;
  }

  if (gameOver) {
    showGameOver();
    return;
  }

  readJoystick();

  if (millis() - lastMoveTime > gameSpeed) {
    moveSnake();
    lastMoveTime = millis();
  }
}

void initGame() {
  snakeLength = 2;
  snakeX[0] = 3; snakeY[0] = 4;
  snakeX[1] = 2; snakeY[1] = 4;
  dirX = 1; dirY = 0;
  gameSpeed = 300;
  spawnFood();
}

void readJoystick() {
  int xVal = analogRead(PIN_X);
  int yVal = analogRead(PIN_Y);

  // X-Axis (Left/Right)
  if (xVal < 200 && dirX == 0) { dirX = -1; dirY = 0; }      // Left
  else if (xVal > 800 && dirX == 0) { dirX = 1; dirY = 0; } // Right
  
  // Y-Axis (Up/Down) - Swapped the < and > signs here to fix reversal
  else if (yVal < 200 && dirY == 0) { dirY = -1; dirX = 0; } // Now Up
  else if (yVal > 800 && dirY == 0) { dirY = 1; dirX = 0; }  // Now Down
  
  // Button (Pause) logic stays the same
  if (digitalRead(PIN_SW) == LOW) {
    tone(BUZZER, 800, 50);
    delay(500); 
    while(digitalRead(PIN_SW) == HIGH) { }
    tone(BUZZER, 1200, 50);
    delay(500);
  }
}

void moveSnake() {
  int newHeadX = snakeX[0] + dirX;
  int newHeadY = snakeY[0] + dirY;

  if (newHeadX < 0 || newHeadX > 7 || newHeadY < 0 || newHeadY > 7) {
    gameOver = true; return;
  }

  for (int i = 0; i < snakeLength; i++) {
    if (newHeadX == snakeX[i] && newHeadY == snakeY[i]) {
      gameOver = true; return;
    }
  }

  if (newHeadX == foodX && newHeadY == foodY) {
    snakeLength++;
    tone(BUZZER, 1000, 40); 
    spawnFood();
    if (gameSpeed > 80) gameSpeed -= 15; 
  }

  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  snakeX[0] = newHeadX;
  snakeY[0] = newHeadY;
  draw();
}

void spawnFood() {
  foodX = random(0, 8);
  foodY = random(0, 8);
}

void draw() {
  lc.clearDisplay(0);
  lc.setLed(0, foodY, foodX, true); 
  for (int i = 0; i < snakeLength; i++) {
    lc.setLed(0, snakeY[i], snakeX[i], true); 
  }
}

void showStartScreen() {
  byte s[8] = {B00111100,B01100000,B01100000,B00111100,B00000110,B00000110,B00111100,B00000000};
  for(int i=0; i<8; i++) lc.setRow(0, i, s[i]);
}

void showGameOver() {
  tone(BUZZER, 150, 600);
  for (int i = 0; i < 2; i++) {
    lc.clearDisplay(0); delay(200);
    for(int r=0; r<8; r++) lc.setRow(0, r, B11111111);
    delay(200);
  }
  gameOver = false;
  gameRunning = false; 
  initGame();
}