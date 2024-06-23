#include <stdio.h>
#include "utility.h"

#include <raylib.h>

#include "map.h"
#include "test_map.h"
#include "pathfinding.h"


int main(){
    printf("Hello, world!\n");

    // PATHFINDING_init(0, 0, 0, 1);
    

    InitWindow(screenWidth, screenHeight, "Map test");
    SetTargetFPS(FPS_LIMIT);
    Color background = {
        .r = 0x30,
        .g = 0x30,
        .b = 0x30
    };

    
    TEST_MAP_crossMap();

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Working in progress", 0, 0, 24, LIGHTGRAY);

        MAP_draw();
        MAP_drawGrid();
        CAR_draw(0, 0);
        CAR_draw(0, 1);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}