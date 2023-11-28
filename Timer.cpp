#include <Time.h>
#include <TimeLib.h>
#include <Arduino.h>

#include "Timer.h"

#define ONE_MINUTE 1 * 60   // In milliseconds
#define FIVE_MINUTE 5 * 60  // In milliseconds

const unsigned int Timer::MAX_TIME = 60 * 60;  // 1 hour in milliseconds
const unsigned int Timer::MIN_TIME = 0;        // 0 seconds

Timer::Timer(unsigned int startTime) {
  lastTime = 0;
  this->startTime = startTime;
  remainingTime = startTime;
  status = TimerStatus::STOPPED;
  previousFormattedValue = "";
}

void Timer::start() {
  status = TimerStatus::ACTIVE;
  lastTime = millis();
}

void Timer::stop() {
  status = TimerStatus::STOPPED;
}

void Timer::toggle() {
  if (isActive()) return stop();

  start();
}

void Timer::reset() {
  status = TimerStatus::STOPPED;
  remainingTime = startTime;
}

bool Timer::isActive() {
  return status == TimerStatus::ACTIVE;
}

bool Timer::isStopped() {
  return status == TimerStatus::STOPPED;
}

void Timer::handle() {
  if (status == TimerStatus::ACTIVE) {
    unsigned int currentTime = millis();

    unsigned int delta = currentTime - lastTime;
    if (delta >= 1000) {
      remainingTime -= delta / 1000;
      lastTime = currentTime;
    }

    if (remainingTime <= Timer::MIN_TIME) {
      return reset();
    }
  }
}

void Timer::addOneMinute() {
  if (remainingTime + ONE_MINUTE <= Timer::MAX_TIME) {
    remainingTime += ONE_MINUTE;
  }
}

void Timer::subOneMinute() {
  if (remainingTime - ONE_MINUTE >= Timer::MIN_TIME) {
    remainingTime -= ONE_MINUTE;
  }
}

void Timer::addFiveMinutes() {
  if (remainingTime + FIVE_MINUTE <= Timer::MAX_TIME) {
    remainingTime += FIVE_MINUTE;
  }
}

void Timer::subFiveMinutes() {
  if (remainingTime - FIVE_MINUTE >= Timer::MIN_TIME) {
    remainingTime -= FIVE_MINUTE;
  }
}

Times Timer::calculateRemainingTime() {
  int minutes = (remainingTime % 3600) / 60;
  int seconds = remainingTime % 60;

  return { minutes, seconds };
}

String Timer::getFormattedValue() {
  Times remaining = calculateRemainingTime();

  String min = String(remaining.minutes);
  String sec = String(remaining.seconds);

  min = remaining.minutes < 10 ? "0" + min : min;
  sec = remaining.seconds < 10 ? "0" + sec : sec;

  return " " + min + ":" + sec;
}

bool Timer::hasChange() {
  String currentFormattedValue = getFormattedValue();

  return currentFormattedValue != previousFormattedValue;
}

String Timer::getValue() {
  String currentFormattedValue = getFormattedValue();

  return previousFormattedValue = currentFormattedValue;
}
