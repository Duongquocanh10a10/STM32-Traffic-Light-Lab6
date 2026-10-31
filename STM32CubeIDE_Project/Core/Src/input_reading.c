#include "input_reading.h"

// TODO (Nguyễn Huỳnh Gia Bửu): Hiện thực chống dội 3 tầng buffer cho PC13 và PC14

void button_reading(void) {
    // Called inside TIM2 ISR every 10ms
}

int is_button_pressed(int index) {
    if (index < 0 || index >= NO_OF_BUTTONS) return 0;
    return 0;
}
