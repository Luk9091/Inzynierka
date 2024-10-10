#include "GUI.h"

void GUI_draw(){
    GUI_drawSpeedPanel();
    GUI_drawAngleSelector();
}



Vector2 GUI_getMousePosition(){
    Vector2 mouse = GetMousePosition();
    mouse.x = (mouse.x - MAP_OFFSET_X) / PIXEL_SIZE_X;
    mouse.y = (mouse.y - MAP_OFFSET_Y) / PIXEL_SIZE_Y;

    return mouse;
}
