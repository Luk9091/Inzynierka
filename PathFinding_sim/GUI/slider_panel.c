#include "GUI.h"
#include "udp.h"
#include <stdio.h>
#include <math.h>

#define PADDING_X 20
#define PADDING_Y 20
static const float minSpeedFraction = 0.1f;

static const int GUI_SPEED_PANEL_WIDTH  =  SCREEN_WIDTH * 2/10;
static const int GUI_SPEED_PANEL_HEIGHT =  SCREEN_HEIGHT * 1/20;

static const int GUI_SPEED_PANEL_X  =      SCREEN_WIDTH - GUI_SPEED_PANEL_WIDTH - PADDING_X;
static const int GUI_SPEED_PANEL_Y  =      SCREEN_HEIGHT - GUI_SPEED_PANEL_HEIGHT - PADDING_Y;



static const int maxAngle = 120;
static const int minAngle = 60;

static const int GUI_ANGLE_PANEL_WIDTH  =  SCREEN_WIDTH * 2/10;
static const int GUI_ANGLE_PANEL_HEIGHT =  SCREEN_HEIGHT * 1/20;

static const int GUI_ANGLE_PANEL_X  =      SCREEN_WIDTH - GUI_ANGLE_PANEL_WIDTH - PADDING_X;
// static const int GUI_ANGLE_PANEL_Y  =       MAP_SIZE_Y * CELL_SIZE_Y * PIXEL_SIZE_Y - 2* GUI_SPEED_PANEL_HEIGHT + MAP_OFFSET_Y;
static const int GUI_ANGLE_PANEL_Y  =      GUI_SPEED_PANEL_Y - GUI_ANGLE_PANEL_HEIGHT - PADDING_Y;
static int angle = 90;


float calculateSpeed(){
    static bool set = false;
    static float speed = 1.f;
    Vector2 mouse = GetMousePosition();
    if (
        (mouse.x > GUI_SPEED_PANEL_X && mouse.x - 10 <= GUI_SPEED_PANEL_X + GUI_SPEED_PANEL_WIDTH + 10) 
        &&
        (mouse.y > GUI_SPEED_PANEL_Y && mouse.y <= GUI_SPEED_PANEL_Y + GUI_SPEED_PANEL_HEIGHT)
        || set
    ){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            set = true;
        }
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && set){
            mouse.x = mouse.x - GUI_SPEED_PANEL_X;
            speed = mouse.x/GUI_SPEED_PANEL_WIDTH;
            if (speed > 1.f) speed = 1.f;
            else if (speed < minSpeedFraction) speed = 0.1;
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && set){
            set = false;
        }
    }

    return speed;
}

int getAngle(){
    return angle;
}
void setAngle(int newAngle){
    angle = newAngle;
}

bool setDeltaAngle(int delta){
    angle = angle - delta;
    if (angle > maxAngle){
        angle = maxAngle;
        return false;
    } else if (angle < minAngle){
        angle = minAngle;
        return false;
    }

    return true;
}

void calculateAngle(){
    static bool set = false;
    static int oldAngle;

    Vector2 mouse = GetMousePosition();
    if (
            (mouse.x> GUI_ANGLE_PANEL_X - 20 && mouse.x <= GUI_ANGLE_PANEL_X + GUI_ANGLE_PANEL_WIDTH + 20) 
            &&
            (mouse.y > GUI_ANGLE_PANEL_Y && mouse.y <= GUI_ANGLE_PANEL_Y + GUI_ANGLE_PANEL_HEIGHT)
            || 
            set
    ){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            oldAngle = angle;
            set = true;
        }
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && set){
            mouse.x = mouse.x - GUI_ANGLE_PANEL_X;
            angle = minAngle + mouse.x/GUI_ANGLE_PANEL_WIDTH* (maxAngle - minAngle);
            if (angle < minAngle){
                angle = minAngle;
            } else if (angle > maxAngle){
                angle = maxAngle;
            }
        }


        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && set){
            udp_send("r %i", angle);
            int delta = oldAngle - angle;
            CAR_changeAngle(-delta);
            set = false;
        }
    }
}

void GUI_drawSpeedPanel(){
    DrawRectangle(
        GUI_SPEED_PANEL_X - 5, GUI_SPEED_PANEL_Y - 5,
        GUI_SPEED_PANEL_WIDTH + 10, GUI_SPEED_PANEL_HEIGHT + 10,
        GRAY
    );

    float speed = (float)calculateSpeed();
    DrawRectangle(GUI_SPEED_PANEL_X, GUI_SPEED_PANEL_Y,
        (GUI_SPEED_PANEL_WIDTH * speed), GUI_SPEED_PANEL_HEIGHT,
        GREEN
    );

    DrawText(
        TextFormat("%4.2f", speed),
        GUI_SPEED_PANEL_X + GUI_SPEED_PANEL_WIDTH * 3 / 10, GUI_SPEED_PANEL_Y + GUI_SPEED_PANEL_HEIGHT / 16,
        GUI_SPEED_PANEL_HEIGHT, WHITE
    );
}


void GUI_drawAngleSelector(){
    DrawRectangle(
        GUI_ANGLE_PANEL_X - 5, GUI_ANGLE_PANEL_Y - 5,
        GUI_ANGLE_PANEL_WIDTH + 10, GUI_ANGLE_PANEL_HEIGHT + 10,
        GRAY
    );

    calculateAngle();
    float duty = (angle - 90)/(float)(maxAngle - minAngle);

    int posX = GUI_ANGLE_PANEL_X + GUI_ANGLE_PANEL_WIDTH/2;
    if (duty < 0){
        duty = -duty;
        posX -= GUI_ANGLE_PANEL_WIDTH * duty;
    }

    DrawRectangle(posX, GUI_ANGLE_PANEL_Y,
        (GUI_ANGLE_PANEL_WIDTH * duty), GUI_ANGLE_PANEL_HEIGHT,
        GREEN
    );

    DrawText(
        TextFormat("%3i", angle), 
        GUI_ANGLE_PANEL_X + GUI_ANGLE_PANEL_WIDTH * 3 / 10, GUI_ANGLE_PANEL_Y + GUI_ANGLE_PANEL_HEIGHT / 16,
        GUI_ANGLE_PANEL_HEIGHT, WHITE
    );
}