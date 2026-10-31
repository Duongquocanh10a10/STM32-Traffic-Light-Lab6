#include "led_display.h"

// TODO (Nguyễn Quang Dũng): Hiện thực các hàm điều khiển LED đơn và quét 4 LED 7 đoạn Anode chung

void update7SEG(int index) {
    switch (index) {
        case 0: /* Display led_buffer[0] */ break;
        case 1: /* Display led_buffer[1] */ break;
        case 2: /* Display led_buffer[2] */ break;
        case 3: /* Display led_buffer[3] */ break;
        default: break;
    }
}

void updateCountdownBuffer(void) {
    led_buffer[0] = countdown_roadA / 10;
    led_buffer[1] = countdown_roadA % 10;
    led_buffer[2] = countdown_roadB / 10;
    led_buffer[3] = countdown_roadB % 10;
}

void clearAll7SEG(void) {
    // Turn off EN0..EN3 (PB12..PB15)
}
