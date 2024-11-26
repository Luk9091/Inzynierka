#include <stdio.h>
#include "pathfinding.h"



int bisector_test(){
    printf("Test\n");

    // printf("   \t");
    // for (int alpha = 0; alpha < 180; alpha += 5){
    //     printf("% 4i\t", alpha);
    // }
    // printf("\n");

    // for (int beta = 0; beta < 180; beta += 5){
    //     printf("% 4i\t", beta);
    //     for (int alpha = 0; alpha < 180; alpha += 5){
    //         float bisectorAngle = TURN_CIRCLE_bisectorAngle(alpha, beta);
    //         printf("%-4.1f\t", bisectorAngle);
    //     }
    //     printf("\n");
    // }

    point_t cors_point = {0, 0};

    for(int x = 0; x < 10; x++){
        point_t bisectorPoint = GEOMETRY_pointOnBisector(45, cors_point, x);
        printf("Point: %i, %i\n", bisectorPoint.x, bisectorPoint.y);
    }

    float angle = 30;
    line_t line = GEOMETRY_lineEquation(angle, cors_point);
    printf("Line: %f, %f, %f\n", line.A, line.B, line.C);


    for (int x = 0; x < 10; x++){
        float distance = GEOMETRY_distancePointToLine(line, (point_t){x, 0});
        printf("%f, ", distance);
    }
    printf("\n");

    return 0;
}