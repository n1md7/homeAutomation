#ifndef Component_h
#define Component_h

#include <Arduino.h>

class Component {
public:
  Component(int pin);

  void on();
  void off();

  bool isOn();
  bool isOff();

  void toggle();

  void setState(bool state);

private:
  int pin;
  int state;
};

#endif
