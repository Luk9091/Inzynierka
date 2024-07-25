#include "pathfinding.h"

point_t direction[4] = {
    {0, -1},// North
    {1, 0}, // East
    {0, 1}, // South
    {-1, 0} // West
};

bool PATHFINDING_isValid(point_t current, point_t next, DIRECTION direction){
    if (next.x < 0 || next.x >= MAP_SIZE_X || next.y < 0 || next.y >= MAP_SIZE_Y){
        return false;
    }

    
    cell_t current_cell = MAP_getCell(current.x, current.y);
    cell_t next_cell = MAP_getCell(next.x, next.y);
    int road = 0;

    switch (direction){
        case NORTH:{
            for (uint i = 0; i < CELL_SIZE_X; i++){
                if (current_cell.type[i][0] != next_cell.type[i][CELL_SIZE_Y - 1] &&
                    !(current_cell.type[i][0] == NONE || next_cell.type[i][CELL_SIZE_Y - 1] == NONE)
                ){
                    return false;
                }
                if (current_cell.type[i][0] == ROAD || current_cell.type[i][0] == NONE){
                    road++;
                }
            }
        } break;
        case EAST:{
            for (uint i = 0; i < CELL_SIZE_Y; i++){
                if (current_cell.type[CELL_SIZE_X - 1][i] != next_cell.type[0][i] &&
                    !(current_cell.type[CELL_SIZE_X - 1][i] == NONE || next_cell.type[0][i] == NONE)
                ){
                    return false;
                }
                if (current_cell.type[CELL_SIZE_X - 1][i] == ROAD || current_cell.type[CELL_SIZE_X - 1][i] == NONE){
                    road++;
                }
            }
        } break;
        case SOUTH:{
            for (uint i = 0; i < CELL_SIZE_X; i++){
                if (current_cell.type[i][CELL_SIZE_Y - 1] != next_cell.type[i][0] &&
                    !(current_cell.type[i][CELL_SIZE_Y - 1] == NONE || next_cell.type[i][0] == NONE)
                ){
                    return false;
                }
                if (current_cell.type[i][CELL_SIZE_Y - 1] == ROAD || current_cell.type[i][CELL_SIZE_Y - 1] == NONE){
                    road++;
                }
            }
        } break;
        case WEST:{
            for (uint i = 0; i < CELL_SIZE_Y; i++){
                if (current_cell.type[0][i] != next_cell.type[CELL_SIZE_X - 1][i] &&
                    !(current_cell.type[0][i] == NONE || next_cell.type[CELL_SIZE_X - 1][i] == NONE)
                ){
                    return false;
                }
                if (current_cell.type[0][i] == ROAD || current_cell.type[0][i] == NONE){
                    road++;
                }
            }
        } break;
    }

    return road > 0;
}

void PATHFINDING_initNodes(node_t nodes[MAP_SIZE_X][MAP_SIZE_Y]){
    for (int x = 0; x < MAP_SIZE_X; x++){
        for (int y = 0; y < MAP_SIZE_Y; y++){
            nodes[x][y].distance = INF;
            nodes[x][y].prev = (point_t){-1, -1};
        }
    }
}


int PATHFINDING_dijkstra(point_t start, point_t destination, DIRECTION preferredDirection, point_t path[MAP_SIZE_X * MAP_SIZE_Y]){
    node_t nodes[MAP_SIZE_X][MAP_SIZE_Y];;
    bool visited[MAP_SIZE_X][MAP_SIZE_Y] = {false};

    PATHFINDING_initNodes(nodes);

    nodes[start.x][start.y].distance = 0;


    for (int count = 0; count < MAP_SIZE_X * MAP_SIZE_Y - 1; count++){
        int min = INF;
        point_t minPoint = {-1, -1};

        for (int x = 0; x < MAP_SIZE_X; x++){
            for (int y = 0; y < MAP_SIZE_Y; y++){
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

        for (int dir = preferredDirection; dir < preferredDirection+4; dir++){
            int i = dir % 4;
            point_t new = {minPoint.x + direction[i].x, minPoint.y + direction[i].y};

            if (PATHFINDING_isValid(minPoint, new, i) && !visited[new.x][new.y] && nodes[minPoint.x][minPoint.y].distance + 1 < nodes[new.x][new.y].distance){
                nodes[new.x][new.y].distance = nodes[minPoint.x][minPoint.y].distance + 1;
                nodes[new.x][new.y].prev = minPoint;
                if (minPoint.x == start.x && minPoint.y == start.y && i != preferredDirection){
                    nodes[new.x][new.y].distance += 20;
                }
            }
        }
    }



    if (nodes[destination.x][destination.y].distance == INF){
        return 0;
    }

    int index = 0;
    for (point_t at = destination; at.x != -1; at = nodes[at.x][at.y].prev) {
        path[index++] = at;
    }

    for (int i = 0; i < index / 2; i++) {
        point_t temp = path[i];
        path[i] = path[index - i - 1];
        path[index - i - 1] = temp;
    }

    return index;
}