// RotaryEncoder by Matthias Hertel
#include <RotaryEncoder.h>
#include <Keyboard.h>

constexpr int PIN_SWITCH[6] = { 4,5,6,7,10,11 };

RotaryEncoder encoder1(A2, A3, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder encoder2(A4, A5, RotaryEncoder::LatchMode::FOUR3);

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(PIN_SWITCH[i], INPUT_PULLUP);
  }
  delay(100);
  Keyboard.begin();
}

void loop() {
  static long encoder1_position = 0;
  static long encoder2_position = 0;
  encoder1.tick();
  encoder2.tick();

  int dir1 = encoder1.getPosition() - encoder1_position;
  if (dir1 > 0) {
    Keyboard.press('l');
    Keyboard.release('l');
  } else if (dir1 < 0) {
    Keyboard.press('r');
    Keyboard.release('r');
  }
  encoder1_position = encoder1.getPosition();

  static unsigned long timer_switch = 0;

  if (millis() - timer_switch > 50) {
    timer_switch = millis();

    int dir2 = encoder2.getPosition() - encoder2_position;
    if (dir2 > 0) {
      Keyboard.press('L');
      Keyboard.release('L');
    } else if (dir2 < 0) {
      Keyboard.press('R');
      Keyboard.release('R');
    }
    encoder2_position = encoder2.getPosition();

    if (digitalRead(PIN_SWITCH[0]) == LOW) {
      Keyboard.press('1');
      Keyboard.release('1');
    }
    if (digitalRead(PIN_SWITCH[1]) == LOW) {
      Keyboard.press('2');
      Keyboard.release('2');
    }
    if (digitalRead(PIN_SWITCH[2]) == LOW) {
      Keyboard.press('3');
      Keyboard.release('3');
    }
    if (digitalRead(PIN_SWITCH[3]) == LOW) {
      Keyboard.press('4');
      Keyboard.release('4');
    }
    if (digitalRead(PIN_SWITCH[4]) == LOW) {
      Keyboard.press('5');
      Keyboard.release('5');
    }
    if (digitalRead(PIN_SWITCH[5]) == LOW) {
      Keyboard.press('6');
      Keyboard.release('6');
    }
  }
}
