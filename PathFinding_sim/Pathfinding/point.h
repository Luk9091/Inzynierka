#ifndef __POINT_H__
#define __POINT_H__

typedef struct {
    int x;
    int y;
} point_t;


typedef struct {
    point_t start;
    point_t end;
    int distance;
    int angle;
    bool isArc;
    point_t center;
    int radius;
    int arcAngle;
} instruction_t;

#endif