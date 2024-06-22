#include "map.h"
#include <raylib.h>

static cell_t map[MAP_SIZE_Y][MAP_SIZE_X];


void MAP_addCell(uint x, uint y, cell_t cell){
    map[y][x] = cell;
}

cell_t MAP_getCell(uint x, uint y){
    return map[y][x];
}
