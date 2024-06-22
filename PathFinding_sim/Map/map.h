#ifndef __MAP__H
#define __MAP__H

#include <stdio.h>
#include <stdlib.h>
#include "../utility.h"

#define MAP_SIZE_X 8
#define MAP_SIZE_Y 8

#define CELL_SIZE_X 8
#define CELL_SIZE_Y 8

#define PIXEL_SIZE_X 2
#define PIXEL_SIZE_Y 2

typedef union cell_t{
    uint64_t cell;
    uint8_t subcell[8][8];
} cell_t;


void MAP_addCell(uint x, uint y, cell_t cell);
cell_t MAP_getCell(uint x, uint y);

void MAP_draw(uint x, uint y);



#endif