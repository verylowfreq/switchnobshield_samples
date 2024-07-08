// RotaryEncoder by Matthias Hertel
#include <RotaryEncoder.h>
#include <Keyboard.h>
#include <Mouse.h>

constexpr int PIN_SWITCH[6] = { 4,5,6,7,10,11 };

RotaryEncoder encoder1(A2, A3, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder encoder2(A4, A5, RotaryEncoder::LatchMode::FOUR3);

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(PIN_SWITCH[i], INPUT_PULLUP);
  }
  delay(100);
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  process_encoders();
  process_switches();
}


/// 回転ノブを処理する関数
void process_encoders() {
  static long encoder1_position = 0;
  static long encoder2_position = 0;
  static unsigned long timer_encoders = 0;

  encoder1.tick();
  encoder2.tick();

  if (millis() - timer_encoders > 10) {
    // 10msごとにキー入力に反映する
    timer_encoders = millis();

    int dir1 = encoder1.getPosition() - encoder1_position;
    if (dir1 > 0) {
      Keyboard.press(KEY_LEFT_ARROW);
      Keyboard.release(KEY_LEFT_ARROW);
    } else if (dir1 < 0) {
      Keyboard.press(KEY_RIGHT_ARROW);
      Keyboard.release(KEY_RIGHT_ARROW);
    }
    encoder1_position = encoder1.getPosition();

    int dir2 = encoder2.getPosition() - encoder2_position;
    if (dir2 > 0) {
      Mouse.move(0, 0, 4);
    } else if (dir2 < 0) {
      Mouse.move(0, 0, -4);
    }
    encoder2_position = encoder2.getPosition();
  }
}


/// スイッチを処理する関数（回転ノブの押し込みも含む）
void process_switches() {
  static unsigned long timer_switch = 0;

  if (millis() - timer_switch > 50) {
    // 50msごとに押下状況を確認して、キー入力に反映する
    timer_switch = millis();


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