#include <LiquidCrystal.h>
#include <Arduino.h>
#include <AbleButtons.h>

#include "Timer.h"
#include "Screen.h"
#include "structs.h"
#include "Component.h"

#define INC_BTN_PIN A0
#define DEC_BTN_PIN A1
#define START_BTN_PIN 7
#define STATUS_LED_PIN 13
#define DEFAULT_TIME 600  // 10 minutes

using Button = AblePullupClickerButton;

LiquidCrystal LCD(9, 8, 5, 4, 3, 2);
const Screen screen(LCD);

const Component status(LED_BUILTIN);
const Component device(5);

const Button increment(INC_BTN_PIN);
const Button decrement(DEC_BTN_PIN);
const Button start(START_BTN_PIN);

const Timer timer(DEFAULT_TIME);


void setup() {

  Serial.begin(9600);

  start.begin();
  increment.begin();
  decrement.begin();
  screen.begin();
}

void loop() {
  start.handle();
  increment.handle();
  decrement.handle();
  timer.handle();

  if (timer.hasChange()) {
    screen.print(timer.getValue(), timer.isActive());
  }

  // resetClicked = click + release event
  if (start.resetClicked()) timer.toggle();  // Start/Pause
  if (increment.resetClicked()) timer.addOneMinute();
  if (decrement.resetClicked()) timer.subOneMinute();


  const bool active = timer.isActive();

  device.setState(active);
  status.setState(active);
}
