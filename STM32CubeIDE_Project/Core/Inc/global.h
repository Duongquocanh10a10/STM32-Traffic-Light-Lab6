#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"
extern UART_HandleTypeDef huart1;
#define huart2 huart1
// --- DEFAULT TIMING PARAMETERS (in seconds) ---
#define TIME_GREEN_DEFAULT      8
#define TIME_AMBER_DEFAULT      2
#define TIME_RED_DEFAULT        10
#define TIME_PED_DEFAULT        5

// --- FINITE STATE MACHINE ENUMS ---
enum SystemMode {
    MODE_NORMAL = 0,
    MODE_NIGHT  = 1,
    MODE_PAUSED = 2
};

enum LightState {
    STATE_INIT,
    S0_A_GREEN_B_RED,       // 8s
    S1_A_AMBER_B_RED,       // 2s
    S2_A_RED_PED_B_GREEN,   // 5s if pedestrian_flag == 1, else skipped
    S3_A_RED_B_GREEN,       // 8s
    S4_A_RED_B_AMBER        // 2s
};

// --- SHARED GLOBAL VARIABLES ---
extern enum SystemMode systemMode;
extern enum LightState lightState;

extern int pedestrian_flag;
extern int countdown_roadA;
extern int countdown_roadB;
extern int led_buffer[4];

extern uint8_t temp_rx;
extern uint8_t buffer_flag;

#endif /* INC_GLOBAL_H_ */
