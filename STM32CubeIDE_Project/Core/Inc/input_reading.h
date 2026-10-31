#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

#include "global.h"

#define NO_OF_BUTTONS       2
#define PED_BUTTON_INDEX    0   // PC13
#define MODE_BUTTON_INDEX   1   // PC14

void button_reading(void);
int is_button_pressed(int index);

#endif /* INC_INPUT_READING_H_ */
