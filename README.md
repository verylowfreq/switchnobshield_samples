# Switch and Knob Shield sample code

<img height=320 src="https://github.com/verylowfreq/switchnobshield_samples/assets/60875431/66abe17b-9fc1-432f-a26c-6c37597879ce">

This repository has sample codes for Switch and Knob Shield.


<img height=320 src="https://github.com/verylowfreq/switchnobshield_samples/assets/60875431/9fad5710-094c-4071-9d57-acd03e94930a">


### arduino_shieldmacropad

The Arduino's code. This code uses "Keyboard" library provided by Arduino which can enable the board to act as USB keyboard.

I checked on Arduino Leonardo. And Arduino UNO R4 should be okay.

### arduino_ch32v_shiledmacropad

The CH32V203's code. CH32V is now lack of the USB-related library, so I modified the official sample code.

Build with Arduino IDE and arduino_core_ch32.

Act as USB keyboard, mouse, and consumer device.


## Pin assign

(Pin name is Arduino Leonardo's one.)

 - Switch 1 : 4
 - Switch 2 : 5
 - Switch 3 : 6
 - Switch 4 : 7
 - Rotary Encoder 1 A : A2
 - Rotary Encoder 1 B : A3
 - Rotary Encoder 1 Push : 10
 - Rotary Encoder 1 A : A4
 - Rotary Encoder 1 B : A5
 - Rotary Encoder 2 Push : 11

