constexpr int PIN_LED_BUILTIN = PA5;
constexpr int PIN_BTN_0 = PA15;
constexpr int PIN_BTN_1 = PB3;
constexpr int PIN_BTN_2 = PB4;
constexpr int PIN_BTN_3 = PB5;
constexpr int PIN_BTN_4 = PA4;
constexpr int PIN_BTN_5 = PA7;
constexpr int PIN_RE1A = PA2;
constexpr int PIN_RE1B = PA3;
constexpr int PIN_RE2A = PB0;
constexpr int PIN_RE2B= PB1;

// RotaryEncoder by Matthias Hertel
#include <RotaryEncoder.h>

#include "src/USB-Driver/inc/usb_lib.h"
#include "src/CONFIG/usb_desc.h"
#include "src/CONFIG/usb_pwr.h"
#include "src/CONFIG/usb_prop.h"
#include "src/CONFIG/hw_config.h"

// WORKAROUND: These function declaration exists in hw_config.h but no effect.
extern "C" {
    void Set_USBConfig(void);
    void USB_Interrupts_Config(void);
    uint8_t USBD_ENDPx_DataUp( uint8_t endp, uint8_t *pbuf, uint16_t len );
    void MCU_Sleep_Wakeup_Operate(void);
}

RotaryEncoder re1(PIN_RE1A, PIN_RE1B, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder re2(PIN_RE2A, PIN_RE2B, RotaryEncoder::LatchMode::FOUR3);


void setup() {
    Serial.begin(115200);
    Serial.println("Initializing...");
    Serial.println("USB-HID composite, Keyboard and Consumer control");
    Serial.printf("SystemCoreClock=%d\n", SystemCoreClock);

    Set_USBConfig();
    USB_Init();
    USB_Interrupts_Config();
    
    Serial.println("Ready.");
}

void led_blink(unsigned int interval_ms) {
    static unsigned long timer = 0;
    static bool led_on = false;

    if (timer == 0) {
        pinMode(PIN_LED_BUILTIN, OUTPUT);
    }

    if (millis() - timer > interval_ms) {
        timer = millis();
        
        digitalWrite(PIN_LED_BUILTIN, led_on ? LOW : HIGH);
        led_on = !led_on;
    }
}


// Keyboard's LED state (updated on USB callbacks)
volatile uint8_t KB_LED_Cur_Status;

uint8_t usbd_kbd_report[8];
uint8_t USBD_KBD_REPORT_BYTES = sizeof(usbd_kbd_report) / sizeof(usbd_kbd_report[0]);

void usbd_kbd_update(void) {
    static unsigned long timer = 0;

    if (timer == 0) {
        pinMode(PIN_BTN_0, INPUT_PULLUP);
        pinMode(PIN_BTN_1, INPUT_PULLUP);
        pinMode(PIN_BTN_2, INPUT_PULLUP);
        pinMode(PIN_BTN_3, INPUT_PULLUP);
    }

    if (millis() - timer >= 10) {
        timer = millis();

        bool btn_w = digitalRead(PIN_BTN_0) == LOW;
        bool btn_a = digitalRead(PIN_BTN_1) == LOW;
        bool btn_s = digitalRead(PIN_BTN_2) == LOW;
        bool btn_d = digitalRead(PIN_BTN_3) == LOW;

        if (btn_w) {
            usbd_kbd_report[3] = 0x1a;
        }
        if (btn_a) {
            usbd_kbd_report[4] = 0x04;
        }
        if (btn_s) {
            usbd_kbd_report[5] = 0x16;
        }
        if (btn_d) {
            usbd_kbd_report[6] = 0x07;
        }

        bool success = USBD_ENDPx_DataUp(1, usbd_kbd_report, USBD_KBD_REPORT_BYTES);
        if (success) {
          memset(usbd_kbd_report, 0x00, USBD_KBD_REPORT_BYTES);
        }
    }
}

/// HID Report for Mouse
/// Layout:
///   - Buttons: bitfield [ 0,0,0,0,0, Button3, Button2, Button1 ]
///   - Cursor X: signed 1 byte integer
///   - Cursor Y: signed 1 byte integer
///   - Wheel vertical: signed 1 byte integer
///   - Wheel horizontal: signed 1 byte integer
uint8_t usbd_mouse_report[5];
uint8_t USBD_MOUSE_REPORT_BYTES = sizeof(usbd_mouse_report) / sizeof(usbd_mouse_report[0]);

void usbd_mouse_update(void) {
    static unsigned long timer = 0;
    static long prevPosition = 0;

    if (timer == 0) {
        memset(usbd_mouse_report, 0x00, USBD_MOUSE_REPORT_BYTES);
    }

    unsigned long current_time = millis();
    if (timer - current_time >= 10) {
        timer = current_time;

        bool btn_left = re1.getPosition() - prevPosition > 0;
        bool btn_right = re1.getPosition() - prevPosition < 0;
        prevPosition = re1.getPosition();

        if (btn_left) {
            usbd_mouse_report[4] = (uint8_t)-1;
        } else if (btn_right) {
            usbd_mouse_report[4] = 1;
        }

        bool success = USBD_ENDPx_DataUp(2, usbd_mouse_report, USBD_MOUSE_REPORT_BYTES);
        if (success) {
            memset(usbd_mouse_report, 0x00, USBD_MOUSE_REPORT_BYTES);
        }
    }
}

uint8_t usbd_cc_report[1];
uint8_t USBD_CC_REPORT_BYTES = sizeof(usbd_cc_report) / sizeof(usbd_cc_report[0]);

void usbd_cc_update(void) {
    static unsigned long timer = 0;
    static long prevPosition = 0;
    static bool muteButtonSent = false;

    if (timer == 0) {
        pinMode(PIN_BTN_5, INPUT_PULLUP);
        memset(usbd_cc_report, 0x00, USBD_CC_REPORT_BYTES);
    }

    if (millis() - timer >= 50) {
        timer = millis();
        bool btn_down = (re2.getPosition() - prevPosition) < 0;
        bool btn_up = (re2.getPosition() - prevPosition) > 0;
        prevPosition = re2.getPosition();
        bool btn_mute = false;
        if (digitalRead(PIN_BTN_5) == LOW) {
          if (!muteButtonSent) {
            btn_mute = true;
            muteButtonSent = true;
          }
        } else {
          muteButtonSent = false;
        }
        
        if (btn_mute) {
            usbd_cc_report[0] = 0x01;
        } else if (btn_down) {
            usbd_cc_report[0] = 0x04;
        } else if (btn_up) {
            usbd_cc_report[0] = 0x08;
        } else {
            usbd_cc_report[0] = 0;
        }

        bool success = USBD_ENDPx_DataUp(3, usbd_cc_report, USBD_CC_REPORT_BYTES);
        if (success) {
          memset(usbd_cc_report, 0x00, USBD_CC_REPORT_BYTES);
        }
    }
}


void loop() {

    if( bDeviceState == CONFIGURED )
    {
        led_blink(500);

        re1.tick();
        re2.tick();

        usbd_kbd_update();
        usbd_mouse_update();
        usbd_cc_update();

    } else {
        led_blink(1000);
    }
}



/** MCUをスリープ状態にする。復帰後の再初期化をする。
*/
void MCU_Sleep_Wakeup_Operate(void) {
    Serial.printf( "Enter to Sleep\r\n" );
    __disable_irq();

    PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);
    
    SystemInit();
    SystemCoreClockUpdate();
    Set_USBConfig();
    
    __enable_irq( );
    Serial.printf( "Wake\r\n" );
}
