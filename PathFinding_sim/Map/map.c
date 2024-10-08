#include "map.h"
#include <raylib.h>
#include <math.h>

static cell_t map[MAP_SIZE_X][MAP_SIZE_Y];


Color _MAP_type2color(cellType_t type){
    switch (type){
        case NONE:
            return DARKBLUE;
        case ROAD:
            return BLACK;
        case WALL:
            return GREEN;
        default:
            return ORANGE;
    }
}

void MAP_fill(cellType_t type){
    cell_t cell;
    MAP_fillCell(&cell, type);

    for(uint i = 0; i < MAP_SIZE_X; i++){
        for(uint j = 0; j < MAP_SIZE_Y; j++){
            MAP_addCell(i, j, cell);
        }
    }
}

void MAP_fillCell(cell_t *cell, cellType_t type){
    Color color = _MAP_type2color(type);
    for(uint y = 0; y < CELL_SIZE_Y; y++){
        for(uint x = 0; x < CELL_SIZE_X; x++){
            cell->type[x][y] = type;
            cell->color[x][y]= color;
        }
    }
}
void MAP_fill_xLine(cell_t *cell, uint x, cellType_t type){
    Color color = _MAP_type2color(type);
    for (uint i = 0; i < CELL_SIZE_Y; i++){
        cell->type[x][i] = type;
        cell->color[x][i]= color;
    }
}

void MAP_fill_xLineToEnd(cell_t *cell, uint start, uint end, cellType_t type){
    for (uint x = start; x <= end; x++){
        MAP_fill_xLine(cell, x, type);
    }
}



void MAP_fill_yLine(cell_t *cell, uint y, cellType_t type){
    Color color = _MAP_type2color(type);
    for (uint i = 0; i < CELL_SIZE_X; i++){
        cell->type[i][y] = type;
        cell->color[i][y]= color;
    }
}
void MAP_fill_yLineToEnd(cell_t *cell, uint start, uint end, cellType_t type){
    for (uint y = start; y <= end; y++){
        MAP_fill_yLine(cell, y, type);
    }
}

cell_t MAP_rotate(cell_t cell, float angle){
    cell_t newCell;
    angle = -(angle * 2*M_PI)/360.f;
    int x, y;
    float new_x, new_y;

    for (uint i = 0; i < CELL_SIZE_X; i++){
        for(uint j = 0; j < CELL_SIZE_Y; j++){
            x = i + 1;//- CELL_SIZE_X/2;
            y = j + 1;//- CELL_SIZE_X/2;


            new_x = roundf(cos(angle)*x - sin(angle)*y);

            new_y = roundf(sin(angle)*x + cos(angle)*y);

            if (new_x < 0) new_x = (CELL_SIZE_X) + new_x; 
            else new_x = new_x - 1;// - CELL_SIZE_X/2;

            if (new_y < 0) new_y = (CELL_SIZE_Y) + new_y; 
            else new_y = new_y - 1;// - CELL_SIZE_Y/2;

            newCell.type[(int)new_x][(int)new_y] = cell.type[i][j];
        }
    }

    return newCell;
}

void MAP_setType(uint x, uint y, cellType_t type){
    uint _x = x / CELL_SIZE_X;
    uint _y = y / CELL_SIZE_Y;
    uint  i = x % CELL_SIZE_X;
    uint  j = y % CELL_SIZE_Y;


    map[_x][_y].color[i][j] = _MAP_type2color(type);
    map[_x][_y].type[i][j] = type;
}

void MAP_addCell(uint x, uint y, cell_t cell){
    for (uint i = 0; i < CELL_SIZE_X; i++){
        for (uint j = 0; j < CELL_SIZE_Y; j++){
            cell.color[i][j] = _MAP_type2color(cell.type[i][j]);
        }
    }
    map[x][y] = cell;
}

cell_t MAP_getCell(uint x, uint y){
    return map[x][y];
}

void MAP_addCost(uint x, uint y, int cost){
    map[x][y].distance = cost;
}

void MAP_drawCell(uint _x, uint _y){
    uint x, y;
    cell_t cell = map[_x][_y];


    for (uint j = 0; j < CELL_SIZE_Y; j++){
        for (uint i = 0; i < CELL_SIZE_X; i++){
            x = _x*CELL_SIZE_X*PIXEL_SIZE_X + i*PIXEL_SIZE_X + MAP_OFFSET_X;
            y = _y*CELL_SIZE_Y*PIXEL_SIZE_Y + j*PIXEL_SIZE_Y + MAP_OFFSET_Y;

            DrawRectangle(x, y, PIXEL_SIZE_X, PIXEL_SIZE_Y, cell.color[i][j]);
        }
    }
}

void MAP_draw(){
    for (uint x = 0; x < MAP_SIZE_X; x++){
        for (uint y = 0; y < MAP_SIZE_Y; y++){
            MAP_drawCell(x, y);
        }
    }
}

void MAP_drawSmallGrid(uint x, uint y){
    uint posX, posY;
    x = x + MAP_OFFSET_X;
    y = y + MAP_OFFSET_Y;

    for (uint _x = 0; _x < CELL_SIZE_X; _x++){
        posX = x + _x * PIXEL_SIZE_X;
        for (uint _y = 0; _y < CELL_SIZE_Y; _y++){
            posY = y + _y * PIXEL_SIZE_Y;

            DrawRectangleLines(
                posX, posY,
                PIXEL_SIZE_X, PIXEL_SIZE_Y,
                GRAY
            );
        }
    }
}


void MAP_drawGrid(){
    uint size_x = CELL_SIZE_X*PIXEL_SIZE_X;
    uint size_y = CELL_SIZE_Y*PIXEL_SIZE_Y;
    for (uint x = 0; x < MAP_SIZE_X; x++){
        for (uint y = 0; y < MAP_SIZE_Y; y++){
            MAP_drawSmallGrid(x*size_x, y*size_y);
            DrawRectangleLines(
                x*size_x + MAP_OFFSET_X,
                y*size_y + MAP_OFFSET_Y,
                size_x,
                size_y,
                GRAY
            );
        }
    }
}

void MAP_drawIndex(){
    int size = PIXEL_SIZE_X*PIXEL_SIZE_Y/2;
    if (size > 16) size = 16;
    for (uint x = 0; x < MAP_SIZE_X; x++){
        for (uint y = 0; y < MAP_SIZE_Y; y++){
            DrawText(
                TextFormat("%d, %d", x, y),
                x*CELL_SIZE_X*PIXEL_SIZE_X + MAP_OFFSET_X,
                y*CELL_SIZE_Y*PIXEL_SIZE_Y + MAP_OFFSET_Y,
                size,
                GRAY
            );
        }
    }
}



void POINT_draw(uint x, uint y, float sub_x, float sub_y, uint pixel_size, Color color){
    x = x * PIXEL_SIZE_X + MAP_OFFSET_X + sub_x * (PIXEL_SIZE_X - pixel_size);
    y = y * PIXEL_SIZE_Y + MAP_OFFSET_Y + sub_y * (PIXEL_SIZE_X - pixel_size);
    DrawRectangle(x, y, pixel_size, pixel_size, color);
}



bool _MAP_collisionDetect(uint map_x, uint map_y, uint cell_x, uint cell_y){
    if (map_x >= MAP_SIZE_X || map_y >= MAP_SIZE_Y) return true;
    if (cell_x >= CELL_SIZE_X || cell_y >= CELL_SIZE_Y) return true;
    if (map_x < 0 || map_y < 0) return true;
    if (cell_x < 0 || cell_y < 0) return true;

    return map[map_x][map_y].type[cell_x][cell_y] == WALL;
}


bool MAP_collisionDetect(uint x, uint y){
    return _MAP_collisionDetect(x/CELL_SIZE_X, y/CELL_SIZE_Y, x%CELL_SIZE_X, y%CELL_SIZE_Y);
}


cellType_t MAP_getType(uint x, uint y){
    uint map_x = x/CELL_SIZE_X;
    uint map_y = y/CELL_SIZE_Y;

    uint cell_x = x%CELL_SIZE_X;
    uint cell_y = y%CELL_SIZE_Y;

    // printf("Map cell: %2i, %2i\n", map_x, map_y);
    // printf("Cell: %2i, %2i\n", cell_x, cell_y);

    cell_t cell = MAP_getCell(map_x, map_y);
    cellType_t type = cell.type[cell_x][cell_y];
    return type;
}