#ifndef __PATHFINDING__H
#define __PATHFINDING__H

#include <stdio.h>
#include <memory.h>
#include <raylib.h>
#include <stdbool.h>
#include <math.h>

#include "map.h"
#include "utility.h"
#include "point.h"
#include "list.h"

#define INF INT16_MAX


typedef struct {
    point_t prev;
    int distance;
    bool visited;
} node_t;


/*
 * Parameters to define a line
 * A * x + B * y + C = 0
*/
typedef struct {
    float A;
    float B;
    float C;
} line_t;




int PATHFINDING_dijkstra(point_t start_map, point_t destination, point_t preferredDirection, list_t *pathList);
int PATHFINDING_fromPathToLinear(list_t *pathList, list_t *instructionList);
void PATHFINDING_connectInstructionWithArc(list_t *instructionList);
void PATHFINDING_removeEmptyInstructions(list_t *instructionList);


point_t GEOMETRY_pointOnLine(float angle, point_t point, float x);
line_t GEOMETRY_lineEquation(float angle, point_t linePoint);
float GEOMETRY_distancePointToLine(line_t line, point_t toPoint);
float  GEOMETRY_bisectorAngle(float angle1, float angle2);

point_t GEOMETRY_tangentPoint(point_t center, int radius, line_t line);
point_t GEOMETRY_pointWithLine(line_t line, int x);
float GEOMETRY_angleBetweenPoints(point_t a, point_t b, point_t c);

// int PATHFINDING_fromPathToInstruction(list_t *pathList, list_t *instructionList);
#endif