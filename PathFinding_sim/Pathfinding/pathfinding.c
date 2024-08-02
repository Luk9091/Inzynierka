#include "pathfinding.h"

point_t direction[] = {
    { 0, -1},   // North
    // { 1, -1},   // North East
    { 1,  0},   // East
    // { 1,  1},   // South East
    { 0,  1},   // South
    // {-1,  1},   // South West
    {-1,  0},   // West
    // {-1, -1}    // North West
};

bool PATHFINDING_isValid(point_t current, point_t dir){
    // printf("Current on index: %4i, %4i\n", current.x, current.y);
    // printf("Next on index: %2i, %2i\n", next.x, next.y);
    point_t next = {
        .x = current.x + dir.x,
        .y = current.y + dir.y
    };

    if (
        current.x < 0 || current.y < 0 || current.x >= MAP_SIZE_X * CELL_SIZE_X || current.y >= MAP_SIZE_Y * CELL_SIZE_Y ||
        next.x < 0 || next.y < 0 || next.x >= MAP_SIZE_X * CELL_SIZE_X || next.y >= MAP_SIZE_Y * CELL_SIZE_Y
    ) {
        return false;
    }

    if (dir.x != 0 && dir.y != 0){
        if (
            MAP_getType(current.x, next.y) == WALL ||
            MAP_getType(next.x, current.y) == WALL 
        ){
            return false;
        }
    }

    cellType_t currentType = MAP_getType(current.x, current.y);
    cellType_t nextType = MAP_getType(next.x, next.y);

    if (nextType != ROAD){
        return false;
    }
    
    if (nextType == NONE || currentType == NONE){
        if (nextType != currentType)
            return false;
    }

    return true;
}

void PATHFINDING_initNodes(node_t nodes[MAP_SIZE_X*CELL_SIZE_X][MAP_SIZE_Y * CELL_SIZE_Y]){
    for (int x = 0; x < MAP_SIZE_X*CELL_SIZE_X; x++){
        for (int y = 0; y < MAP_SIZE_Y*CELL_SIZE_Y; y++){
            nodes[x][y].distance = INF;
            nodes[x][y].prev = (point_t){-1, -1};
        }
    }
}


int PATHFINDING_dijkstra(point_t start, point_t destination, point_t preferredDirection, point_t path[MAP_SIZE_X * MAP_SIZE_Y * CELL_SIZE_X * CELL_SIZE_Y]){
    node_t nodes[MAP_SIZE_X*CELL_SIZE_X][MAP_SIZE_Y*CELL_SIZE_Y];
    bool visited[MAP_SIZE_X*CELL_SIZE_X][MAP_SIZE_Y*CELL_SIZE_Y] = {false};

    PATHFINDING_initNodes(nodes);

    nodes[start.x][start.y].distance = 0;


    for (int count = 0; count < MAP_SIZE_X * MAP_SIZE_Y * CELL_SIZE_X * CELL_SIZE_Y - 1; count++){
        int min = INF;
        point_t minPoint = {-1, -1};

        for (int x = 0; x < MAP_SIZE_X * CELL_SIZE_X; x++){
            for (int y = 0; y < MAP_SIZE_Y * CELL_SIZE_Y; y++){
                if (!visited[x][y] && nodes[x][y].distance <= min){
                    min = nodes[x][y].distance;
                    minPoint = (point_t){x, y};
                }
            }
        }

        if (minPoint.x == -1 || minPoint.y == -1){
            break;
        }
        visited[minPoint.x][minPoint.y] = true;

        for (int i = 0; i < sizeof(direction) / sizeof(direction[0]); i++){
            point_t new = {minPoint.x + direction[i].x, minPoint.y + direction[i].y};
            if (PATHFINDING_isValid(minPoint, direction[i])){
                int distance = abs(direction[i].x) + abs(direction[i].y);
                if (preferredDirection.x != direction[i].x || preferredDirection.y != direction[i].y){
                    distance = distance * 4;
                }
                if ( !visited[new.x][new.y] && nodes[minPoint.x][minPoint.y].distance + distance < nodes[new.x][new.y].distance){
                    nodes[new.x][new.y].distance = nodes[minPoint.x][minPoint.y].distance + distance;
                    nodes[new.x][new.y].prev = minPoint;
                    preferredDirection = direction[i];
                }
            }
        }
    }



    if (nodes[destination.x][destination.y].distance == INF){
        return 0;
    }

    int index = 0;
    for (point_t at = destination; at.x != -1; at = nodes[at.x][at.y].prev) {
        // printf("At: %i, %i\n", at.x, at.y);
        path[index++] = at;
    }

    for (int i = 0; i < index / 2; i++) {
        point_t temp = path[i];
        path[i] = path[index - i - 1];
        path[index - i - 1] = temp;
    }

    return index;
}