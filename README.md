## Timer based high voltage controller

### Components

- Arduino(Mega) (https://store.arduino.cc/arduino-mega-2560-rev3)
- LCD Display (16x2)
- Real Time Clock(DS1307)
- Keypad(4x3)
- Relay module(5DC-240VAC-30A)

<a href="https://youtu.be/KvfRnmXLC6U">Live video</a>

## How to use

### Compile and upload

Check your arduino port with `arduino-cli board list` and replace `/dev/cu.usbmodem144101` with your port
```bash
arduino-cli board list
# or
ls -l /dev/*.* | grep usb
```

```bash
arduino-cli compile --profile mega
arduino-cli upload --port /dev/cu.usbmodem144101 --profile mega
```

```bash
arduino-cli compile --profile uno
arduino-cli upload --port /dev/cu.usbmodem144101 --profile uno
```

__NOTE__: All dependencies are installed automatically

## Manual step by step

### Compile

Prerequisites:

- arduino-cli installed (https://arduino.github.io/arduino-cli/latest/installation/)
- arduino:avr:mega board installed (`arduino-cli core install arduino:avr`)
- LiquidCrystal library installed (`arduino-cli lib install LiquidCrystal`)
- Time library installed (`arduino-cli lib install Time`)

__NOTE__: The board can be changed to any other board that is supported by the arduino-cli

```bash
arduino-cli compile -b arduino:avr:mega high-voltage-timer.ino
```

### Upload

```bash
arduino-cli upload -p /dev/cu.usbmodem144101 -b arduino:avr:mega high-voltage-timer.ino
```

__NOTE__: The port can be changed to the port that the arduino is connected to (`/dev/cu.usbmodem144101` is the port for
my arduino)
