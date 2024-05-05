#include "map.h"
#include <raylib.h>

static cell_t map[MAP_SIZE_Y][MAP_SIZE_X];


void MAP_addCell(uint x, uint y, cell_t cell){
    map[y][x] = cell;
}

cell_t MAP_getCell(uint x, uint y){
    return map[y][x];
}


// Color getColor(cell_t cell, uint8_t x, uint8_t y){
//     if (cell.y[y] & (1 << x)){
//         return GREEN;
//     }
//     return BLACK;
// }

// void map_draw(uint x, uint y){
//     for (uint yp = 0; yp < CELL_SIZE_Y; yp++){
//         for (uint xp = 0; xp < CELL_SIZE_X; xp++){
//             DrawRectangle(x*CELL_SIZE_X + xp*PIXEL_SIZE_X, y*CELL_SIZE_Y + y*PIXEL_SIZE_Y, PIXEL_SIZE_X, PIXEL_SIZE_Y, getColor(map[y][x], xp, yp));
//         }
//     }
// }
