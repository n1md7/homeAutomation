#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>
#include <Arduino.h>

struct LED {
  int green;
  int red;
  int neutral;
};

struct Status {
  bool on;
};

struct Elapsed {
  int minutes;
  int seconds;
};

struct Timer {
  int value;
  int screen;
};

struct Timestamp {
  int switchBtn;
  int incrementBtn;
  int decrementBtn;
};

struct Pressed {
  bool switchBtn;
  bool incrementBtn;
  bool decrementBtn;
};

const int buttonPin0 = A0;  // Increment Button
const int buttonPin1 = A1;  // Decrement Button
const int buttonPin2 = A2;  // Switch (ON/OFF) Button

const int BUTTON_PRESS_DELAY = 50;
const int SCREEN_UPDATE_DELAY = 100;
const int DEFAULT_TIME = 600; // 10 Minutes
const int HOUR_IN_SECONDS = 3600;
const int MINUTE_IN_SECONDS = 60;

unsigned long prevTick;

LiquidCrystal screen(9, 8, 5, 4, 3, 2);
Timer timer = { DEFAULT_TIME - 1, millis() };
Timestamp timestamp = { -1000, -1000, -1000 };
Pressed pressed = { false, false, false };
Status pumpIs = { false };
LED led = { A5, A4, 7 };

// Define custom characters (Pixel)
byte a[8] = { B00100, B00100, B00010, B00001, B00001, B10001, B10001, B01110 };
byte C[8] = { B11110, B10001, B10010, B11100, B10000, B11110, B10001, B10001 };
byte r[8] = { B10000, B10000, B10000, B10000, B11110, B10001, B10001, B10001 };
byte T[8] = { B01010, B10101, B10101, B10101, B10101, B10101, B10101, B01001 };
byte u[8] = { B11011, B00101, B00001, B00001, B00001, B10001, B10001, B01110 };
byte l[8] = { B01110, B10001, B10001, B10000, B10000, B01000, B10100, B01111 };
byte i[8] = { B01110, B10001, B10001, B10001, B10001, B10001, B10001, B10001 };

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin0, INPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  screen.begin(16, 2);
  screen.clear();
  screen.print("Hi...");
  delay(2000);
  screen.clear();
  screen.print("Created by n1md7");
  screen.setCursor(2, 2);
  screen.print("03.07.2017");
  delay(2000);
  screen.clear();

  // Defining custom characters for LCD. Max chars are restricted upto 7.
  screen.createChar(1, C);
  screen.createChar(2, a);
  screen.createChar(3, r);
  screen.createChar(4, T);
  screen.createChar(5, u);
  screen.createChar(6, l);
  screen.createChar(7, i);

  pinMode(buttonPin0, INPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  pinMode(led.red, OUTPUT);
  pinMode(led.green, OUTPUT);
  pinMode(led.neutral, OUTPUT);

  prevTick = now();
}

Elapsed calculateTime(int elapsedTime) {
  Elapsed elapsed;
  elapsed.minutes = elapsedTime / 60;
  elapsed.seconds = elapsedTime % 60;

  return elapsed;
}

// Function to calculate minutes and seconds from elapsed time
String getFormattedTime(Elapsed time) {
  // Convert minutes and seconds to string
  String minutesStr = String(time.minutes);
  String secondsStr = String(time.seconds);

  // Pad the strings with leading zeros if necessary
  if (time.minutes < 10) minutesStr = "0" + minutesStr;
  if (time.seconds < 10) secondsStr = "0" + secondsStr;

  // Return the formatted time string
  return " " + minutesStr + ":" + secondsStr;
}

void printPumpIsOn() {
  // Print: "ჩართულია" on LCD Screen
  screen.print(char(1));
  screen.print(char(2));
  screen.print(char(3));
  screen.print(char(4));
  screen.print(char(5));
  screen.print(char(6));
  screen.print(char(7));
  screen.print(char(2));
}

void printPumpIsPaused() {
  screen.print("Paused: ");
}

void printStatusIsOn() {
  screen.print(" Status: ON");
}

void printStatusIsOff() {
  screen.print("Status: OFF");
}

void updateScreen(int time) {
  screen.setCursor(0, 0);

  if (pumpIs.on) printPumpIsOn();
  else printPumpIsPaused();

  Elapsed elapsedTime = calculateTime(time);
  String formattedTime = getFormattedTime(elapsedTime);

  screen.print(formattedTime);
  screen.setCursor(2, 1);

  if (pumpIs.on) printStatusIsOn();
  else printStatusIsOff();
}

void updateBtnTimestamps() {
  if (digitalRead(buttonPin2) == HIGH) timestamp.switchBtn = millis();
  else if (digitalRead(buttonPin0) == HIGH) timestamp.incrementBtn = millis();
  else if (digitalRead(buttonPin1) == HIGH) timestamp.decrementBtn = millis();
}

void evaluateBtnStates() {
  int currentTimestamp = millis();
  if (currentTimestamp - timestamp.switchBtn < BUTTON_PRESS_DELAY) {
    if (!pressed.switchBtn) {
      pressed.switchBtn = true;
      pumpIs.on = !pumpIs.on;
    }
  } else pressed.switchBtn = false;
  if (currentTimestamp - timestamp.incrementBtn < BUTTON_PRESS_DELAY && timer.value < HOUR_IN_SECONDS) {
    if (!pressed.incrementBtn) {
      pressed.incrementBtn = true;
      timer.value += 60;  // +1 Minute
    }
  } else pressed.incrementBtn = false;
  if (currentTimestamp - timestamp.decrementBtn < BUTTON_PRESS_DELAY && timer.value > MINUTE_IN_SECONDS) {
    if (!pressed.decrementBtn) {
      pressed.decrementBtn = true;
      timer.value -= 60;  // -1 Minute
    }
  } else pressed.decrementBtn = false;
}

void updateLEDStatus() {
  if (pumpIs.on) {
    digitalWrite(led.neutral, 1);
    digitalWrite(led.green, 1);
    digitalWrite(led.red, 0);
  } else {
    digitalWrite(led.neutral, 0);
    digitalWrite(led.green, 0);
    digitalWrite(led.red, 1);
  }
}

void resetTimer() {
  pumpIs.on = false;
  timer.value = DEFAULT_TIME;
}

void evaluateTicks() {
  int delta = now() - prevTick;
  if (delta > 0) {
    timer.value -= delta;  // Decrement by delta
    prevTick = now();
  }
}

void evaluateScreenUpdate(){
  if (millis() - timer.screen > SCREEN_UPDATE_DELAY) {
    updateScreen(timer.value);
    timer.screen = millis();
  }
}

void loop() {
  if (pumpIs.on) evaluateTicks();
  if (timer.value <= 0) resetTimer();

  updateBtnTimestamps();
  evaluateBtnStates();
  updateLEDStatus();
  evaluateScreenUpdate();
}
