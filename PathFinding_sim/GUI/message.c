#include "GUI.h"
#include <string.h>
#include <memory.h>

#define FONT_SIZE   20
static const int FONT_POS_X = SCREEN_WIDTH * 8 / 10;
static const int FONT_POS_Y = 0.25 * FONT_SIZE;
static int _line = 0;

void GUI_print(const char *str, int line){
    if (line == -1){
        line = _line++;
    } else {
        line++;
    }
    DrawText(
        str,
        FONT_POS_X, FONT_POS_Y + line * FONT_SIZE,
        FONT_SIZE, WHITE
    );
}


void GUI_resetLine(){
    _line = 0;
}