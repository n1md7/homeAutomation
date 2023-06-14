#ifndef STRUCTS_H
#define STRUCTS_H

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

#endif // STRUCTS_H
