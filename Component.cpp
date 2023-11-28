#include "Component.h"

Component::Component(int pin) {
  this->pin = pin;
  this->state = LOW;

  pinMode(pin, OUTPUT);
}

void Component::on() {
  if (isOn()) return;

  state = HIGH;
  digitalWrite(pin, state);
}

void Component::off() {
  if (isOff()) return;

  state = LOW;
  digitalWrite(pin, state);
}

bool Component::isOn() {
  return state == HIGH;
}

bool Component::isOff() {
  return state == LOW;
}

void Component::toggle() {
  if (isOn()) return off();

  on();
}

void Component::setState(bool state) {
  this->state = state;
}
