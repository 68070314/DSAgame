
// Every key press is sent to the PC as one character over USB serial.
// The game decides what each key means (see src/input.c), so this sketch
// never needs to change when the controls change.

#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad pad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);          // must match BAUD_RATE in src/main.c
}

void loop() {
  char key = pad.getKey();     // returns once per press, already debounced
  if (key) {
    Serial.write(key);
  }
}
