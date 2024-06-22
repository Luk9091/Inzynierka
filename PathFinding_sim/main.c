#include <stdio.h>
#include "utility.h"

#include <raylib.h>


#include "map.h"


int main(){
    printf("Hello, world!\n");

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
        DrawText("Working in progress", 0, 0, 24, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}