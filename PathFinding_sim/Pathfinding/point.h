#ifndef __POINT_H__
#define __POINT_H__

typedef struct {
    int x;
    int y;
} point_t;


typedef struct {
    point_t start;
    int distance;
    int angle;
    bool isArc;
    point_t center;
} instruction_t;

#endif