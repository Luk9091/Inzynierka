#ifndef __MAP__H
#define __MAP__H

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include <raylib.h>

#define MAP_SIZE_X 9
#define MAP_SIZE_Y 9

#define CELL_SIZE_X 8
#define CELL_SIZE_Y 8

#define PIXEL_SIZE_X 2
#define PIXEL_SIZE_Y 2

typedef enum{
    NONE,
    ROAD,
    WALL
} cellType_t;

typedef struct{
    cellType_t type[CELL_SIZE_X][CELL_SIZE_Y];
    Color color[CELL_SIZE_X][CELL_SIZE_Y];
    int distance;
} cell_t;

void MAP_fill(cellType_t type);
void MAP_fillCell(cell_t *cell, cellType_t type);

void MAP_fill_xLine(cell_t *cell, uint x, cellType_t type);
void MAP_fill_xLineToEnd(cell_t *cell, uint start, uint end, cellType_t type);

void MAP_fill_yLine(cell_t *cell, uint y, cellType_t type);
void MAP_fill_yLineToEnd(cell_t *cell, uint start, uint end, cellType_t type);

/*
 * Rotate cell
 * @param angle in degree
*/
cell_t MAP_rotate(cell_t cell, float angle);

void MAP_addCell(uint x, uint y, cell_t cell);
cell_t MAP_getCell(uint x, uint y);


void MAP_drawCell(uint x, uint y);
void MAP_draw();
void MAP_drawGrid();

void CAR_draw();


#endif