#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "global.h"

void setTrafficLight_RoadA(int red, int amber, int green);
void setTrafficLight_RoadB(int red, int amber, int green);
void setPedestrianLight(int pedA, int pedB);
void toggleNightAmberLEDs(void);
void turnOffAllTrafficLEDs(void);

void update7SEG(int index);
void updateCountdownBuffer(void);
void clearAll7SEG(void);

#endif /* INC_LED_DISPLAY_H_ */
