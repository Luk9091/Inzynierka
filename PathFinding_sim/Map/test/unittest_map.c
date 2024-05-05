#include "unittest_map.h"
#include <stdio.h>
#include "cell_types.h"

#ifdef _WIN32
#include <inttypes.h>
#define PASSED "PASSED"
#define FAILED "FAILED" 

#ifdef _WIN64
#define PRI_SIZET PRIu64
#else
#define PRI_SIZET PRIu32
#endif
#else
#define PRI_SIZET "zu"
#define PASSED "\x1b[32mPASSED\x1b[0m"
#define FAILED "\x1b[31mFAILED\x1b[0m"
#endif

bool addCell(){
    bool status = false;
    
    printf("Make cell TEST 1\n");
    MAP_addCell(0, 0, MAP_CELL_up_down);
    status = MAP_getCell(0, 0).cell == MAP_CELL_up_down.cell;
    if(status){
        printf("Test 1: ");
        printf(PASSED);
        printf("\n");
    } else {
        printf (FAILED);
        printf("\tExpected value incorrect\n");
        return false;
    }

    return true;
}


bool MAP_unittest(){
    bool status = true;

    status = addCell();
    if (status == false){
        printf(FAILED);
        return false;
    }

    return status;
}