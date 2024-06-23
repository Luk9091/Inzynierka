#ifndef __PATHFINDING__H
#define __PATHFINDING__H

#include <stdio.h>
#include <memory.h>
#include <raylib.h>
#include <stdbool.h>

#include "map.h"
#include "utility.h"

#define INF INT16_MAX

typedef struct {
    int x;
    int y;
} point_t;

typedef enum DIRECTION_t{
    NORTH = 0,
    EAST  = 1,
    SOUTH = 2,
    WEST  = 3
} DIRECTION;

typedef struct {
    point_t prev;
    int distance;
} node_t;


int PATHFINDING_dijkstra(point_t start, point_t destination, DIRECTION preferredDirection, point_t path[MAP_SIZE_X * MAP_SIZE_Y]);



#endif