# Switch and Knob Shield sample code

<img height=320 src="https://github.com/verylowfreq/switchnobshield_samples/assets/60875431/66abe17b-9fc1-432f-a26c-6c37597879ce">

This repository has sample codes for Switch and Knob Shield.

マクロパッドシールド基板のサンプルコードです。

スイッチ4つと回転ノブ2つを、通常のシールド基板サイズに実装しました。マクロパッドや各種入力インターフェースにご利用ください。

<img height=320 src="https://github.com/verylowfreq/switchnobshield_samples/assets/60875431/9fad5710-094c-4071-9d57-acd03e94930a">


### arduino_shieldmacropad

The Arduino's code. This code uses "Keyboard" library provided by Arduino which can enable the board to act as USB keyboard.

I checked on Arduino Leonardo and UNO R4. Please note that UNO R3 cannot be used as USB keyboard.

Please install "RotaryEncoder by Matthias Hertel" library.

USB機能を持つArduinoボードで、Keyboardライブラリを持つボードのためのコードです。Arduino Leonardo, UNO R4など。

ライブラリ "RotaryEncoder by Matthias Hertel" をインストールして、ビルドしてください。

**注意： UNO R3はUSBキーボードとしては利用できません。**

### suzuno32rv_shiledmacropad

The CH32V203's code for Suzuno32RV, Suzuduino UNO.

Build with Arduino IDE and arduino_core_ch32_sz.

Act as USB keyboard, mouse, and consumer device.

Please install "RotaryEncoder by Matthias Hertel" library.

CH32V203向けのコードです。Suzuno32RV, Suzuduino UNOで動作します。TinyUSBを利用します。

ライブラリ "RotaryEncoder by Matthias Hertel" をインストールして、ビルドしてください。


## Pin assign

The functions and pin assignments.

機能とピンの割り当てです。

| 機能 | Leonardo | Suzuno32RV |
|---|---|---|
| Switch 1 | 4 | PA15 |
| Switch 2 | 5 | PB3 |
| Switch 3 | 6 | PB4 |
| Switch 4 | 7 | PB5 |
| Rotary Encoder 1 A | A2 | PA2 |
| Rotary Encoder 1 B | A3 | PA3 |
| Rotary Encoder 1 Push (SW5) | 10 | PA4 |
| Rotary Encoder 2 A | A4 | PB0 |
| Rotary Encoder 2 B | A5 | PB1 |
| Rotary Encoder 2 Push (SW6) | 11 | PA7 |
