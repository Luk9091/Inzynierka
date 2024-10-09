#ifndef __UTILITIES__H
#define __UTILITIES__H

#include <memory.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "printf_color.h"

typedef unsigned int uint;

#define π 3.14159f

#define screenWidth 1280
#define screenHeight 1010
#define FPS_LIMIT 60


#define MAP_SIZE_X 3
#define MAP_SIZE_Y 3

#define CELL_SIZE_X 8
#define CELL_SIZE_Y 8

#define PIXEL_SIZE_X 32
#define PIXEL_SIZE_Y 32

#define MAP_OFFSET_X 2
#define MAP_OFFSET_Y 0

// Distance in mm
#define DISTANCE_PER_PIXEL 100

#define DEVICE "PC"


float rounds(float value, uint decimalPlace);
int atoli(char *str, int *numbers);

extern int sleep_ms(uint miliseconds);
float mapValue(float value, float fromMin, float fromMax, float toMin, float toMax);
#endif