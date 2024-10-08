#ifndef __MAP__H
#define __MAP__H

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include <raylib.h>


typedef enum{
    NONE,
    ROAD,
    WALL
} cellType_t;

typedef struct CELL_t{
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
void MAP_addCost(uint x, uint y, int cost);


void MAP_drawCell(uint x, uint y);
void MAP_draw();
void MAP_drawGrid();
void MAP_drawIndex();

/*
 * \param x absolut coordination of X on map
 * \param y absolut coordination of Y on map
 *
 * For example: x = 10, y = 0
 * this mean:
 * cell address: 1, 0
 * point in cell: 2, 0
 * 
*/
void MAP_setType(uint x, uint y, cellType_t type);

void POINT_draw(uint x, uint y, float sub_x, float sub_y, uint pixel_size, Color color);

// bool MAP_collisionDetect(uint map_x, uint map_y, uint cell_x, uint cell_y);
bool MAP_collisionDetect(uint x, uint y);
cellType_t MAP_getType(uint x, uint y);

#endif