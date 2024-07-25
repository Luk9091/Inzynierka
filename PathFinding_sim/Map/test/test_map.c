#include "test_map.h"

cell_t TEST_MAP_streetCell(){
    cell_t cell;
    MAP_fillCell(&cell, WALL);

    MAP_fill_xLineToEnd(&cell, 2, 5, ROAD);

    return cell;
}

cell_t TEST_MAP_XcrossCell(){
    cell_t cell;
    MAP_fillCell(&cell, WALL);

    MAP_fill_xLineToEnd(&cell, 2, 5, ROAD);
    MAP_fill_yLineToEnd(&cell, 2, 5, ROAD);

    return cell;
}

cell_t TEST_MAP_TcrossCell(){
    cell_t cell = TEST_MAP_XcrossCell();
    MAP_fill_xLineToEnd(&cell, 0, 1, WALL);

    return cell;
}

cell_t TEST_MAP_turnCell(){
    cell_t cell = TEST_MAP_XcrossCell();
    MAP_fill_xLineToEnd(&cell, 0, 1, WALL);
    MAP_fill_yLineToEnd(&cell, 0, 1, WALL);
    return cell;
}

cell_t TEST_MAP_wallCell(){
    cell_t cell;
    MAP_fillCell(&cell, WALL);
    return cell;
}

void TEST_MAP_circleMap(){
    MAP_fill(WALL);
    cell_t street_y = TEST_MAP_streetCell();
    cell_t street_x = MAP_rotate(TEST_MAP_streetCell(), 90);

    MAP_addCell(0, 0, MAP_rotate(TEST_MAP_turnCell(),   0));
    MAP_addCell(0, MAP_SIZE_Y - 1, MAP_rotate(TEST_MAP_turnCell(),  90));
    MAP_addCell(MAP_SIZE_X - 1, MAP_SIZE_Y - 1, MAP_rotate(TEST_MAP_turnCell(), 180));
    MAP_addCell(MAP_SIZE_X - 1, 0, MAP_rotate(TEST_MAP_turnCell(), 270));

    for (uint i = 1; i <= MAP_SIZE_X - 2; i++){
        MAP_addCell(i, 0, street_x);
        MAP_addCell(i, MAP_SIZE_X - 1, street_x);
    }

    for (uint i = 1; i <= MAP_SIZE_Y - 2; i++){
        MAP_addCell(0, i, street_y);
        MAP_addCell(MAP_SIZE_Y - 1, i, street_y);
    }
}

void TEST_MAP_crossMap(){
    cell_t street_y = TEST_MAP_streetCell();
    cell_t street_x = MAP_rotate(TEST_MAP_streetCell(), 90);
    uint center_x = MAP_SIZE_X / 2;
    uint center_y = MAP_SIZE_Y / 2;

    TEST_MAP_circleMap();
    

    for(uint i = 1; i <= MAP_SIZE_X-2; i++){
        MAP_addCell(i, 4, street_x);
    }
    for(uint i = 1; i <= MAP_SIZE_Y-2; i++){
        MAP_addCell(4, i, street_y);
    }

    MAP_addCell(center_x, center_y, TEST_MAP_XcrossCell());
    MAP_addCell(0, center_y, TEST_MAP_TcrossCell());
    MAP_addCell(center_x, MAP_SIZE_Y - 1, MAP_rotate(TEST_MAP_TcrossCell(), 90));
    MAP_addCell(MAP_SIZE_X - 1, center_y, MAP_rotate(TEST_MAP_TcrossCell(), 180));
    MAP_addCell(center_x, 0, MAP_rotate(TEST_MAP_TcrossCell(), 270));
}