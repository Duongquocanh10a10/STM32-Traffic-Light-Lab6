#include "fsm_traffic.h"
#include "input_reading.h"
#include "software_timer.h"
#include "led_display.h"
#include "uart_comm.h"

// TODO (Nguyễn Huỳnh Gia Bửu): Hiện thực Mode FSM và Light Cycle FSM (S0 -> S4, S2 cộng dồn 5s)

void fsm_for_mode(void) {
    if (is_button_pressed(MODE_BUTTON_INDEX)) {
        switch (systemMode) {
            case MODE_NORMAL: systemMode = MODE_NIGHT;  break;
            case MODE_NIGHT:  systemMode = MODE_PAUSED; break;
            case MODE_PAUSED: systemMode = MODE_NORMAL; break;
        }
    }
}

void fsm_for_light_cycle(void) {
    // Switch-case S0..S4
}
