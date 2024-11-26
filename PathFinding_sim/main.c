#include <stdio.h>
#include "utility.h"

#include <raylib.h>
#include <pthread.h>
#include "queue.h"

#include "GUI.h"
#include "printf_color.h"
#include "map.h"
#include "test_map.h"

#include "car.h"
#include "udp.h"

#define CAR_START_POS_X 11.5
#define CAR_START_POS_Y 11.5
#define CAR_START_ANGLE 0
static float changeAngle  = 0.f;

void keyRead();
void readRecv();

int main(){
    // pthread_t UDP_thread;
    // pthread_create(&UDP_thread, NULL, udp_run, NULL);

    // TEST_MAP_crossMap();
    TEST_MAP_empty();

    CAR_init(CAR_START_POS_X, CAR_START_POS_Y, CAR_START_ANGLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Map test");
    SetTargetFPS(FPS_LIMIT);
    Color background = {
        .r = 0x30,
        .g = 0x30,
        .b = 0x30
    };

    while (!WindowShouldClose()){
        keyRead();
        // readRecv();

        BeginDrawing();
        ClearBackground(DARKGRAY);

        MAP_draw();
        MAP_drawGrid(true);
        MAP_drawIndex();
        GUI_draw();
        
        CAR_draw();

        Vector2 mouse = GUI_getMousePosition();
        GUI_print(TextFormat("Mouse: %2i, %2i", (int)mouse.x, (int)mouse.y), -1);
        GUI_print(TextFormat("Angle: %6.2f", changeAngle), -1);

        EndDrawing();
        GUI_resetLine();
    }

    // pthread_detach(UDP_thread);
    udp_server_stop();
    CloseWindow();

    CAR_free();

    return 0;
}
// */


void moveByPath(){
    CAR_moveByPath();
    udp_send("r %i", getAngle());
    udp_send("d %i", DISTANCE_PER_PIXEL);
    udp_send("f");
    udp_send("s %.2f", mapValue(calculateSpeed(), 0.1f, 1.f, 0.1f, 0.5f));
    udp_send("\n");
    changeAngle = 0.f;
}


void moveByInstruction(){
    instruction_t instruction = CAR_moveByInstruction();

    printf("Instruction: %i, %i, %i, %i\n", instruction.start.x, instruction.start.y, instruction.end.x, instruction.end.y);

    if (instruction.isArc){
        printf("Arc\n");

    } else {
        printf("Line\n");
    }

}




void keyRead(){
    if (IsKeyPressed(KEY_W) ^ IsKeyPressedRepeat(KEY_W)){
        CAR_move();
        udp_send("d %i", DISTANCE_PER_PIXEL);
        udp_send("s %.2f", mapValue(calculateSpeed(), 0.1f, 1.f, 0.1f, 0.5f));
        udp_send("r %i", getAngle());
        udp_send("f");
        udp_send("\n");
        changeAngle = 0.f;
    }
    if (IsKeyPressed(KEY_A) ^ IsKeyPressedRepeat(KEY_A)){
        const int angle = 15;
        if (setDeltaAngle(angle)){
            CAR_changeAngle(-angle);
        }
        udp_send("r %i", getAngle());
        changeAngle = 0.f;

    }
    if (IsKeyPressed(KEY_D) ^ IsKeyPressedRepeat(KEY_D)){
        const int angle = -15;
        if (setDeltaAngle(angle)){
            CAR_changeAngle(-angle);
        }
        udp_send("r %i", getAngle());
        changeAngle = 0.f;
    }
    if (IsKeyPressed(KEY_S) ^ IsKeyPressedRepeat(KEY_S)){
        CAR_moveBackward();
        udp_send("d %i", DISTANCE_PER_PIXEL);
        udp_send("s %.2f", mapValue(calculateSpeed(), 0.1f, 1.f, 0.1f, 0.5f));
        udp_send("r %i", getAngle());
        udp_send("b");
        udp_send("\n");
        changeAngle = 0.f;
    }
    if (IsKeyPressed(KEY_BACKSPACE)){
        setAngle(90);
        CAR_setAngle(CAR_START_ANGLE);
        CAR_setPosition(CAR_START_POS_X, CAR_START_POS_Y);
        changeAngle = 0.f;
    }

    if (IsKeyPressed(KEY_SPACE) ^ IsKeyPressedRepeat(KEY_SPACE)){
        // moveByPath();
        moveByInstruction();
    }

    if (IsKeyPressed(KEY_M)){
        udp_send("m");
        CAR_setBeamDistance(0, 0, 0);
    }
    if (IsKeyPressed(KEY_I)){
        udp_send("i");
    }

    Vector2 mouse = GUI_getMousePosition();
    if (
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && 
        !(
            mouse.x >= MAP_SIZE_X * CELL_SIZE_X ||
            mouse.y >= MAP_SIZE_Y * CELL_SIZE_Y
        )
    ){
        int pathSteps = 0;
        if (!IsKeyDown(KEY_LEFT_SHIFT)){
            pathSteps = CAR_findPath(mouse.x, mouse.y);
        } else {
            pathSteps = CAR_addPath(mouse.x, mouse.y);
        }

        if (pathSteps == 0){
            printf(T_RED "No path found\n" T_RESET);
        } 
    }
}


void readRecv(){
    char msg[UDP_PACKET_SIZE];
    while(udp_tryRead(msg)){
        switch (msg[0]){
            case 'M':{
                int distance[3];
                atoli(msg + 1, distance);
                CAR_setBeamDistance(distance[0], distance[1], distance[2]);
            } break;
            case 'C':{
                changeAngle =  atof(msg+2);
            }
        }
    }
}