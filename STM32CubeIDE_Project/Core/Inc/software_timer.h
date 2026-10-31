#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define TIMER_CYCLE     10  // 10ms matches TIM2 interrupt period
#define MAX_TIMERS      4   // 0: State FSM, 1: 1s Countdown, 2: 7SEG Scan, 3: 1Hz Blink

extern int timer_flag[MAX_TIMERS];

void setTimer(int index, int duration_ms);
void timer_run(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
