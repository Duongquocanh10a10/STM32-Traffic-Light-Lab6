#include "global.h"

enum SystemMode systemMode = MODE_NORMAL;
enum LightState lightState = STATE_INIT;

int pedestrian_flag = 0;
int countdown_roadA = TIME_GREEN_DEFAULT;
int countdown_roadB = TIME_RED_DEFAULT;
int led_buffer[4]   = {0, 8, 1, 0};

uint8_t temp_rx     = 0;
uint8_t buffer_flag = 0;
