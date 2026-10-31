#ifndef INC_UART_COMM_H_
#define INC_UART_COMM_H_

#include "global.h"

void uart_rx_enqueue(uint8_t byte);
void command_parser_fsm(void);
void log_state_transition(const char *roadA, const char *roadB, int countdown);

#endif /* INC_UART_COMM_H_ */
