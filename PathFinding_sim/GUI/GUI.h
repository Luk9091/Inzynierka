#ifndef __GUI_H__
#define __GUI_H__
#include <raylib.h>

#include "utility.h"
#include "car.h"

void GUI_draw();
Vector2 GUI_getMousePosition();

void GUI_drawSpeedPanel();
float calculateSpeed();

void GUI_drawAngleSelector();
void calculateAngle();
int getAngle();
void setAngle(int newAngle);
bool setDeltaAngle(int delta);


void GUI_print(const char *str, int line);
void GUI_resetLine();

#endif