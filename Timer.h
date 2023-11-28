#ifndef Timer_h
#define Timer_h

#include "TimerStatus.h"

struct Times {
  int minutes;
  int seconds;
  bool hasChanged();
};

class Timer {
public:
  Timer(unsigned int startTime);

  void start();
  void stop();
  void reset();
  bool isActive();
  bool isStopped();
  bool hasChange();
  void handle();

  void addOneMinute();
  void subOneMinute();

  void addFiveMinutes();
  void subFiveMinutes();

  void toggle();

  String getValue();

  static const unsigned int MAX_TIME;
  static const unsigned int MIN_TIME;

private:
  TimerStatus status;

  unsigned int startTime;
  unsigned int remainingTime;
  unsigned int lastTime;

  String previousFormattedValue;

  Times calculateRemainingTime();
  String getFormattedValue();
};

#endif
