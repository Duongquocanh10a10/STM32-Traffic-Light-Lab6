#include "uart_comm.h"
#include <stdio.h>

// TODO (Trần Ngọc Hùng Anh): Hiện thực Command Parser nhận lệnh MODE=n và hàm truyền UART log

void uart_rx_enqueue(uint8_t byte) {
    // Lưu byte vào buffer và bật buffer_flag = 1
}

void command_parser_fsm(void) {
    // Phân tích lệnh MODE=0, MODE=1, MODE=2
}

void log_state_transition(const char *roadA, const char *roadB, int countdown) {
    // char message[40];
    // int length = sprintf(message, "!STATE:A=%s,B=%s,T=%d#\r\n", roadA, roadB, countdown);
    // HAL_UART_Transmit(&huart2, (uint8_t *)message, length, 100);
}
