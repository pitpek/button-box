#include <Arduino.h>
#include <Keyboard.h>

constexpr uint8_t kButtonCount = 5;
const uint8_t buttonPins[kButtonCount] = {6, 7, 8, 9, 10};

constexpr uint8_t kToggleCount = 2;
const uint8_t togglePins[kToggleCount] = {14, 15};

constexpr uint8_t kPotCount = 2;
const uint8_t kPotPins[kPotCount] = {A0, A1};
constexpr unsigned long kPotLogIntervalMs = 5000;
constexpr unsigned long kDebounceMs = 25;
constexpr unsigned long kDisplayHotkeyCooldownMs = 280;

uint8_t readMask(const uint8_t* pins, uint8_t n) {
  uint8_t m = 0;
  for (uint8_t i = 0; i < n; i++) {
    if (digitalRead(pins[i]) == LOW) {
      m |= static_cast<uint8_t>(1u << i);
    }
  }
  return m;
}

struct Debouncer {
  uint8_t update(uint8_t sample, unsigned long nowMs) {
    if (sample != lastSample_) {
      lastSample_ = sample;
      settleStartMs_ = nowMs;
    }
    if ((nowMs - settleStartMs_) >= kDebounceMs) {
      stable_ = lastSample_;
    }
    return stable_;
  }

private:
  uint8_t stable_ = 0;
  uint8_t lastSample_ = 0xFF;
  unsigned long settleStartMs_ = 0;
};

void sendCtrlAlt(char key) {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(static_cast<uint8_t>(key));
  delay(30);
  Keyboard.releaseAll();
}

void printAnalogPin(uint8_t pin) {
  switch (pin) {
    case A0:
      Serial.print(F("A0"));
      break;
    case A1:
      Serial.print(F("A1"));
      break;
    default:
      Serial.print(pin);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  for (uint8_t i = 0; i < kButtonCount; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  for (uint8_t i = 0; i < kToggleCount; i++) {
    pinMode(togglePins[i], INPUT_PULLUP);
  }
}

void loop() {
  const unsigned long now = millis();

  static Debouncer btnDeb;
  static Debouncer tglDeb;
  const uint8_t btnMask = btnDeb.update(readMask(buttonPins, kButtonCount), now);
  const uint8_t tglMask = tglDeb.update(readMask(togglePins, kToggleCount), now);

  static uint8_t prevBtn = 0xFF;
  static unsigned long hotkeyOkMs[2] = {0, 0};

  if (btnMask != prevBtn) {
    const uint8_t changed = static_cast<uint8_t>(btnMask ^ prevBtn);
    prevBtn = btnMask;

    for (uint8_t i = 0; i < kButtonCount; i++) {
      if ((changed & static_cast<uint8_t>(1u << i)) == 0) {
        continue;
      }
      const bool down = (btnMask & static_cast<uint8_t>(1u << i)) != 0;

      if (down) {
        if (i == 0 && now >= hotkeyOkMs[0]) {
          sendCtrlAlt('1');
          hotkeyOkMs[0] = now + kDisplayHotkeyCooldownMs;
        } else if (i == 1 && now >= hotkeyOkMs[1]) {
          sendCtrlAlt('4');
          hotkeyOkMs[1] = now + kDisplayHotkeyCooldownMs;
        }
      }

      Serial.print(F("button "));
      Serial.print(static_cast<uint8_t>(i + 1));
      Serial.print(F(" pin "));
      Serial.print(buttonPins[i]);
      Serial.println(down ? F(" down") : F(" up"));
    }
  }

  static uint8_t prevTgl = 0xFF;
  if (tglMask != prevTgl) {
    const uint8_t changed = static_cast<uint8_t>(tglMask ^ prevTgl);
    prevTgl = tglMask;

    for (uint8_t i = 0; i < kToggleCount; i++) {
      if ((changed & static_cast<uint8_t>(1u << i)) == 0) {
        continue;
      }
      Serial.print(F("toggle "));
      Serial.print(static_cast<uint8_t>(i + 1));
      Serial.print(F(" pin "));
      Serial.print(togglePins[i]);
      const bool on = (tglMask & static_cast<uint8_t>(1u << i)) != 0;
      Serial.println(on ? F(" on") : F(" off"));
    }
  }

  static unsigned long potNextLogMs = 0;
  if (now >= potNextLogMs) {
    potNextLogMs = now + kPotLogIntervalMs;
    for (uint8_t i = 0; i < kPotCount; i++) {
      const uint8_t pin = kPotPins[i];
      if (i > 0) {
        delayMicroseconds(200);
      }
      const uint16_t v = analogRead(pin);
      Serial.print(F("pot "));
      Serial.print(static_cast<uint8_t>(i + 1));
      Serial.print(F(" pin "));
      printAnalogPin(pin);
      Serial.print(F(" "));
      Serial.println(v);
    }
  }
}
