#include "Screen.h"
#include "custom-characters.h"


Screen::Screen(LiquidCrystal& LCD) {
  this->LCD = LCD;
  status = ScreenStatus::AWAKE;

  defineCustomChars();
}

void Screen::begin() {
  LCD.begin(16, 2);

  authorGreeting();
}

void Screen::sleep() {
  status = ScreenStatus::SLEEP;
}

void Screen::wakeUp() {
  status = ScreenStatus::AWAKE;
}

void Screen::setState(bool state) {
  state ? wakeUp() : sleep();
}

void Screen::authorGreeting() {
  LCD.clear();
  LCD.print("Hi...");

  delay(2000);

  LCD.clear();
  LCD.print("Created by n1md7");
  LCD.setCursor(2, 2);
  LCD.print("03.07.2017");

  delay(2000);

  LCD.clear();
}

void Screen::defineCustomChars() {
  // Define custom characters for LCD.
  // Max chars are restricted to 8.
  LCD.createChar(1, C);
  LCD.createChar(2, a);
  LCD.createChar(3, r);
  LCD.createChar(4, T);
  LCD.createChar(5, u);
  LCD.createChar(6, l);
  LCD.createChar(7, i);
}

void Screen::print(String time, bool isActive) {
  LCD.setCursor(0, 0);
  isActive ? printDeviceIsOn() : printDeviceIsPaused();
  LCD.print(time);
  LCD.setCursor(2, 1);
  isActive ? printStatusIsOn() : printStatusIsOff();
}

void Screen::printDeviceIsOn() {
  // Print: "ჩართულია" on the LCD Screen
  LCD.print(char(1));
  LCD.print(char(2));
  LCD.print(char(3));
  LCD.print(char(4));
  LCD.print(char(5));
  LCD.print(char(6));
  LCD.print(char(7));
  LCD.print(char(2));
}

void Screen::printDeviceIsPaused() {
  LCD.print("Paused: ");
}

void Screen::printStatusIsOn() {
  LCD.print(" Status: ON");
}

void Screen::printStatusIsOff() {
  LCD.print("Status: OFF");
}
