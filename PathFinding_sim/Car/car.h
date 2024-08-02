#ifndef __CAR_H__
#define __CAR_H__

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>

#include "utility.h"
#include "pathfinding.h"
#include "map.h"

#define CAR_SIZE_X 16/2
#define CAR_SIZE_Y 16/2
#define CAR_SEE_RANGE 6



typedef struct{
    Vector2 position;
    // Vector2 step;
    float angle;
    Color color;

    Vector2 top;
    Vector2 left;
    Vector2 right;
} car_t;

// extern car_t car;

void CAR_init(float x, float y, float direction);
void CAR_draw();
void CAR_drawBeam(float angle, float radius);
Vector2 CAR_measureDistance(float angle);



void CAR_setAngle(float angle);
void CAR_changeAngle(float angle);
void CAR_setPosition(int x, int y);


bool CAR_move();
uint CAR_moveByPath();
int CAR_findPath(uint x, uint y);

#endif