#ifndef __PATHFINDING__H
#define __PATHFINDING__H

#include <stdio.h>
#include <memory.h>
#include <raylib.h>

#include "utility.h"

typedef struct {
    void *node;
    float angle;
    int distance;
} next_node_t;

typedef struct {
    uint x;
    uint y;
    
    next_node_t *next;
    uint children;
} node_t;



void PATHFINDING_init(uint start_x, uint start_y, uint end_x, uint end_y);



#endif