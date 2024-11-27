#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <memory.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "printf_color.h"

typedef unsigned int uint;

#define π 3.14159f

#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   960
#define FPS_LIMIT       60


#define MAP_SIZE_X 3
#define MAP_SIZE_Y 3

#define CELL_SIZE_X 8
#define CELL_SIZE_Y 8

#define PIXEL_SIZE 32

#define MAP_OFFSET_X 2
#define MAP_OFFSET_Y 0

// Minimal turn radius in mm
#define MIN_RADIUS (350) 

// Distance in mm
#define DISTANCE_PER_PIXEL (100)

// Odległość przedniej osi od tylnej w mm
static const float CAR_PHYSICAL_LEN = 150;
static const float CAR_PHYSICAL_WIDTH = 125;

#define DEVICE "PC"


float rounds(float value, uint decimalPlace);
int atoli(char *str, int *numbers);

extern int sleep_ms(uint miliseconds);
float mapValue(float value, float fromMin, float fromMax, float toMin, float toMax);

int sign(int value);
int signf(float value);

inline static int max(int a, int b){
    return a > b ? a : b;
}
inline static int min(int a, int b){
    return a < b ? a : b;
}
#endif // __UTILITY_H__