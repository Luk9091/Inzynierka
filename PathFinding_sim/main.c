#include <stdio.h>
#include "utility.h"

#include <raylib.h>

#include "printf_color.h"
#include "map.h"
#include "test_map.h"
#include "car.h"

int main(){
    printf("Hello, world!\n");
    TEST_MAP_crossMap();
    point_t path[MAP_SIZE_X * MAP_SIZE_Y * CELL_SIZE_X * CELL_SIZE_Y] = {0};

    CAR_init(2.5, 2.5, 0);

    InitWindow(screenWidth, screenHeight, "Map test");
    SetTargetFPS(FPS_LIMIT);
    Color background = {
        .r = 0x30,
        .g = 0x30,
        .b = 0x30
    };

    double lastTime = GetTime();
    double currentTime;
    while (!WindowShouldClose()){
        Vector2 mouse = GetMousePosition();
        mouse.x /= 8;
        mouse.y /= 8;
        if (IsKeyPressed(KEY_W) ^ IsKeyPressedRepeat(KEY_W)){
            CAR_move();
        }
        if (IsKeyPressed(KEY_A) ^ IsKeyPressedRepeat(KEY_A)){
            CAR_changeAngle(-15);
        }
        if (IsKeyPressed(KEY_D) ^ IsKeyPressedRepeat(KEY_D)){
            CAR_changeAngle(15);
        }
        if (IsKeyPressed(KEY_S) ^ IsKeyPressedRepeat(KEY_S)){
            CAR_changeAngle(180);
            CAR_move();
            CAR_changeAngle(180);
        }
        if (IsKeyPressed(KEY_BACKSPACE)){
            CAR_setAngle(0);
        }

        currentTime = GetTime();
        if (currentTime - lastTime >= 0.1){
        // if (IsKeyPressed(KEY_SPACE) ^ IsKeyPressedRepeat(KEY_SPACE)){
            CAR_moveByPath();
            lastTime = currentTime;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            int pathSteps = CAR_findPath(mouse.x, mouse.y);
            if (pathSteps == 0){
                printf(T_RED "No path found\n" T_RESET);
            } 
            // else {
            //     for (int i = 0; i < pathSteps; i++){
            //         printf("Path[%d]: %d, %d\n", i, path[i].x, path[i].y);
            //     }
            // }
        }


        BeginDrawing();
        ClearBackground(DARKGRAY);
        // DrawText("Working in progress", 0, 0, 24, LIGHTGRAY);

        MAP_draw(MAP_OFFSET_X, MAP_OFFSET_Y);
        MAP_drawGrid(MAP_OFFSET_X, MAP_OFFSET_Y);
        MAP_drawIndex(MAP_OFFSET_X, MAP_OFFSET_Y);
        
        
        
        CAR_draw();
        CAR_drawBeam(0, 50);
        CAR_drawBeam(30, 50);
        CAR_drawBeam(-30, 50);

        DrawText(
            TextFormat("Mouse, %2i, %2i", (int)mouse.x, (int)mouse.y),
            584, 76, 
            20, WHITE
        );

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
// */