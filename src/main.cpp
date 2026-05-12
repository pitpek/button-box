#include <Arduino.h>
#include <Keyboard.h>

constexpr uint8_t ROWS = 4;
constexpr uint8_t COLS = 4;

const uint8_t rowPins[ROWS] = {2, 3, 4, 5};
const uint8_t colPins[COLS] = {6, 7, 8, 9};

const char keyMap[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'},
};

void sendCtrlAltNumber(uint8_t number) {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(static_cast<uint8_t>('0' + number));
  delay(30);
  Keyboard.releaseAll();
}

void runModeHotkeyForKey(char key) {
  if (key == '1') {
    sendCtrlAltNumber(1);
  } else if (key == '2') {
    sendCtrlAltNumber(2);
  } else if (key == '3') {
    sendCtrlAltNumber(3);
  } else if (key == 'A') {
    sendCtrlAltNumber(4);
  }
}

char readKey() {
  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t i = 0; i < ROWS; i++) {
      pinMode(rowPins[i], OUTPUT);
      digitalWrite(rowPins[i], HIGH);
    }

    digitalWrite(rowPins[r], LOW);
    delayMicroseconds(8);

    for (uint8_t c = 0; c < COLS; c++) {
      if (digitalRead(colPins[c]) == LOW) {
        return keyMap[r][c];
      }
    }
  }

  return '\0';
}

void setup() {
  Keyboard.begin();

  for (uint8_t r = 0; r < ROWS; r++) {
    pinMode(rowPins[r], OUTPUT);
    digitalWrite(rowPins[r], HIGH);
  }
  for (uint8_t c = 0; c < COLS; c++) {
    pinMode(colPins[c], INPUT_PULLUP);
  }
}

void loop() {
  static char lastKey = '\0';
  static unsigned long lastChangeMs = 0;
  constexpr unsigned long debounceMs = 25;
  static bool waitingRelease = false;
  static unsigned long cooldownUntilMs = 0;

  char key = readKey();
  unsigned long now = millis();

  if (waitingRelease) {
    if (key == '\0') {
      waitingRelease = false;
    } else {
      return;
    }
  }

  if (now < cooldownUntilMs) {
    return;
  }

  if (key != lastKey && (now - lastChangeMs) > debounceMs) {
    lastChangeMs = now;
    lastKey = key;

    if (key != '\0') {
      if (key == '1' || key == '2' || key == '3' || key == 'A') {
        runModeHotkeyForKey(key);
        waitingRelease = true;
        cooldownUntilMs = millis() + 250;
      }
    }
  }
}
