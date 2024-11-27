#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <math.h>
#include "point.h"
#include "list.h"
#include "utility.h"

/*
 * Parameters to define a line
 * A * x + B * y + C = 0
*/
typedef struct {
    float A;
    float B;
    float C;
} line_t;


point_t GEOMETRY_pointOnLine(float angle, point_t point, float x);
line_t GEOMETRY_lineEquation(float angle, point_t linePoint);
float GEOMETRY_distancePointToLine(line_t line, point_t toPoint);
float  GEOMETRY_bisectorAngle(float angle1, float angle2);

point_t GEOMETRY_tangentPoint(point_t center, int radius, line_t line);
point_t GEOMETRY_pointWithLine(line_t line, int x);
float GEOMETRY_angleBetweenPoints(point_t a, point_t b, point_t c);


#endif // __GEOMETRY_H__