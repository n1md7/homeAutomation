#ifndef Screen_h
#define Screen_h

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "ScreenStatus.h"

class Screen {
public:
  Screen(LiquidCrystal& LCD);

  void sleep();
  void wakeUp();

  void begin();

  void setState(bool state);

  void print(String time, bool isActive);

private:
  LiquidCrystal& LCD;
  ScreenStatus status;

  void authorGreeting();
  void defineCustomChars();

  void printDeviceIsOn();
  void printDeviceIsPaused();
  void printStatusIsOn();
  void printStatusIsOff();
};

#endif
