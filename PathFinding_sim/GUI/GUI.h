#ifndef __GUI_H__
#include <raylib.h>

#include "utility.h"
#include "car.h"


void GUI_drawSpeedPanel();
float calculateSpeed();

void GUI_drawAngleSelector();
void calculateAngle();
int getAngle();
void setAngle(int newAngle);
bool setDeltaAngle(int delta);

#endif