#include <stdio.h>
#include "utility.h"

#include <raylib.h>

#include "printf_color.h"
#include "map.h"
#include "test_map.h"
#include "pathfinding.h"


int main(){
    printf("Hello, world!\n");
    TEST_MAP_crossMap();
    point_t path[MAP_SIZE_X * MAP_SIZE_Y] = {0};
    point_t start = {0, 2};
    point_t end = {4, 1};

    int pathStep = PATHFINDING_dijkstra(start, end, NORTH, path);

    if (pathStep == 0){
        printf(T_RED "No path found\n" T_RESET);
    } else {
        for (int i = 0; i < pathStep; i++){
        printf("Path[%d]: %d, %d\n", i, path[i].x, path[i].y);
        }
    }
    
    InitWindow(screenWidth, screenHeight, "Map test");
    SetTargetFPS(FPS_LIMIT);
    Color background = {
        .r = 0x30,
        .g = 0x30,
        .b = 0x30
    };

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(DARKGRAY);
        // DrawText("Working in progress", 0, 0, 24, LIGHTGRAY);

        MAP_draw(MAP_OFFSET_X, MAP_OFFSET_Y);
        MAP_drawGrid(MAP_OFFSET_X, MAP_OFFSET_Y);
        MAP_drawIndex(MAP_OFFSET_X, MAP_OFFSET_Y);
        
        
        for (int i = 0; i < pathStep; i++){
            CAR_draw(path[i].x, path[i].y, ORANGE);
        }
        CAR_draw(start.x, start.y, BLUE);
        CAR_draw(end.x, end.y, PURPLE);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}