#ifndef __CAR_H__
#define __CAR_H__

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>

#include "GUI.h"
#include "utility.h"
#include "pathfinding.h"
#include "map.h"
#include "udp.h"

#define CAR_SIZE_X (16/2)
#define CAR_SIZE_Y (16/2)
#define CAR_SEE_RANGE 6

// Odległość przedniej osi od tylnej w mm
static const float CAR_PHYSICAL_LEN = 135 / PIXEL_SIZE_X;



typedef struct{
    Vector2 position;
    float angle;
    Color color;

    Vector2 top;
    Vector2 left;
    Vector2 right;
} car_t;


void CAR_init(float x, float y, float direction);
void CAR_draw();

void CAR_drawBeam(float angle, float radius);
void CAR_setBeamDistance(int left, int center, int right);

Vector2 CAR_measureDistance(float angle);



void CAR_setAngle(float angle);
float CAR_getAngle();
void CAR_changeAngle(float angle);
void CAR_setPosition(float x, float y);
Vector2 CAR_getPosition();


bool CAR_move();
bool CAR_moveBackward();
uint CAR_moveByPath();

int CAR_findPath(uint x, uint y);
int CAR_addPath(uint x, uint y);

#endif