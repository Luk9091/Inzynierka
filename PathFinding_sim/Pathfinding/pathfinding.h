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
#include "geometry.h"

#define INF INT16_MAX


typedef struct {
    point_t prev;
    int distance;
    bool visited;
} node_t;




int PATHFINDING_dijkstra(point_t start_map, point_t destination, point_t preferredDirection, list_t *pathList);
int PATHFINDING_fromPathToLinear(list_t *pathList, list_t *instructionList);
void PATHFINDING_connectInstructionWithArc(list_t *instructionList);
void PATHFINDING_removeEmptyInstructions(list_t *instructionList);



// int PATHFINDING_fromPathToInstruction(list_t *pathList, list_t *instructionList);
#endif