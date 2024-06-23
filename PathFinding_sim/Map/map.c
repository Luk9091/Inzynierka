#include "map.h"
#include <raylib.h>
#include <math.h>

static cell_t map[MAP_SIZE_X][MAP_SIZE_Y];


Color _MAP_type2color(cellType_t type){
    switch (type){
        case NONE:
            return RED;
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

void MAP_drawCell(uint _x, uint _y, int offset_x, int offset_y){
    uint x, y;
    cell_t cell = map[_x][_y];


    for (uint j = 0; j < CELL_SIZE_Y; j++){
        for (uint i = 0; i < CELL_SIZE_X; i++){
            x = _x*CELL_SIZE_X*CELL_SIZE_X + i*CELL_SIZE_X + offset_x;
            y = _y*CELL_SIZE_Y*CELL_SIZE_Y + j*CELL_SIZE_Y + offset_y;

            DrawRectangle(x, y, CELL_SIZE_X, CELL_SIZE_Y, cell.color[i][j]);
        }
    }
}

void MAP_draw(int offset_x, int offset_y){
    for (uint x = 0; x < MAP_SIZE_X; x++){
        for (uint y = 0; y < MAP_SIZE_Y; y++){
            MAP_drawCell(x, y, offset_x, offset_y);
        }
    }
}

void MAP_drawGrid(int offset_x, int offset_y){
    for (uint x = 0; x < MAP_SIZE_X; x++){
        for (uint y = 0; y < MAP_SIZE_Y; y++){
            DrawRectangleLines(
                x*CELL_SIZE_X*CELL_SIZE_X + offset_x,
                y*CELL_SIZE_Y*CELL_SIZE_Y + offset_y,
                CELL_SIZE_X*CELL_SIZE_X,
                CELL_SIZE_Y*CELL_SIZE_Y,
                BLACK
            );
        }
    }
}

void MAP_drawIndex(int offset_x, int offset_y){
    for (uint x = 0; x < MAP_SIZE_X; x++){
        for (uint y = 0; y < MAP_SIZE_Y; y++){
            DrawText(
                TextFormat("%d, %d", x, y),
                x*CELL_SIZE_X*CELL_SIZE_X + offset_x,
                y*CELL_SIZE_Y*CELL_SIZE_Y + offset_y,
                10,
                BLACK
            );
        }
    }
}


void CAR_draw(uint x, uint y, int offset_x, int offset_y){
    x = x*CELL_SIZE_X*CELL_SIZE_X + 2.5*CELL_SIZE_X + offset_x;
    y = y*CELL_SIZE_Y*CELL_SIZE_Y + 2.5*CELL_SIZE_Y + offset_y;
    DrawRectangle(
        x, y,
        3*CELL_SIZE_X, 3*CELL_SIZE_Y,
        ORANGE
    );
}