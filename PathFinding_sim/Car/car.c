#include "car.h"

static car_t car;
point_t path[MAP_SIZE_X * MAP_SIZE_Y * CELL_SIZE_X * CELL_SIZE_Y] = {0};
static uint step;
static int pathLen;


void _CAR_moveTriangle(){
    Vector2 center = {
        .x  = car.position.x * CELL_SIZE_X - cosf(car.angle) * CELL_SIZE_X * CELL_SIZE_X / 8 +MAP_OFFSET_X,
        .y  = car.position.y * CELL_SIZE_Y - sinf(car.angle) * CELL_SIZE_Y * CELL_SIZE_Y / 8 +MAP_OFFSET_Y
    };

    car.top.x = center.x + cosf(car.angle) * CAR_SIZE_X;
    car.top.y = center.y + sinf(car.angle) * CAR_SIZE_Y;

    car.left.x = center.x + cosf(car.angle + M_PI_2) * CAR_SIZE_X/2; 
    car.left.y = center.y + sinf(car.angle + M_PI_2) * CAR_SIZE_Y/2;

    car.right.x = center.x + cosf(car.angle - M_PI_2) * CAR_SIZE_X/2;
    car.right.y = center.y + sinf(car.angle - M_PI_2) * CAR_SIZE_Y/2;

}


void _CAR_drawMessage(const char *str){
    const static int fontSize = 20;
    static int height = fontSize/2;
    int width = screenWidth - MAP_SIZE_X*CELL_SIZE_X * 3;

    DrawText(str, width, height, fontSize, WHITE);
}

void _CAR_drawPath(){
    for (int i = step; i < pathLen; i += 1){
        POINT_draw(path[i].x, path[i].y, ORANGE);
    }
}


void CAR_init(float x, float y, float direction){
    car.position.x = x;
    car.position.y = y;
    car.color = BLUE;

    CAR_setAngle(direction);
}


Vector2 CAR_measureDistance(float angle){
    angle = (angle - 90) * DEG2RAD + car.angle;
    
    point_t start = (point_t){
        .x = (int)car.top.x,
        .y = (int)car.top.y
    };


    cell_t cell = MAP_getCell(car.position.x, car.position.y);
}


void CAR_draw(){
    _CAR_drawPath();


    DrawTriangle(
        car.left, car.top, car.right,
        RED
    );

    
    const char *str = TextFormat("Car pos: %2.2f, %2.2f\nAngle: %2.2f\nDestination: %2i, %2i", car.position.x, car.position.y, car.angle * RAD2DEG, path[pathLen-1].x, path[pathLen-1].y);
    _CAR_drawMessage(str);
}

void CAR_drawBeam(float angle, float radius){
    angle = (angle) * DEG2RAD + car.angle;

    Vector2 end = {
        .x = car.top.x + radius * cosf(angle),
        .y = car.top.y + radius * sinf(angle)
    };
    DrawLineV(car.top, end, WHITE);
}

void CAR_setAngle(float angle){
    // angle = angle - 90;
    if (angle > 360){
        angle -= 360;
    } else if (angle < 0){
        angle += 360;
    }
    car.angle = angle * DEG2RAD;
    _CAR_moveTriangle();
}

void CAR_changeAngle(float angle){
    CAR_setAngle((car.angle * RAD2DEG) + angle);
}

void CAR_setPosition(int x, int y){
    car.position.x = x;
    car.position.y = y;
    _CAR_moveTriangle();
}

bool CAR_move(){
    bool collision = false;
    Vector2 move = {
        .x = rounds(cosf(car.angle), 2),
        .y = rounds(sinf(car.angle), 2)
    };
    Vector2 newPos = {
        .x = car.position.x + move.x,
        .y = car.position.y + move.y
    };

    if (MAP_collisionDetect(newPos.x, car.position.y)){
        newPos.x = roundf(car.position.x) - move.x/100;
        collision = true;
    } 
    if (MAP_collisionDetect(car.position.x, newPos.y)){
        newPos.y = roundf(car.position.y) - move.y/100;
        collision = true;
    } 


    car.position = newPos;
    
    _CAR_moveTriangle();
    return !collision;
}

uint CAR_moveByPath(){
    const static float maxAngle = 20 * DEG2RAD;
    if (step < pathLen){
        int x = (int)(path[step].x) - (int)(car.position.x);
        int y = (int)(path[step].y) - (int)(car.position.y);
        float newAngle = rounds(atan2f(y, x), 2);
        
        float delta = newAngle - car.angle;

        if (delta > π) {delta -= 2*π;}
        if (delta < -π){delta += 2*π;}


        if (delta > maxAngle) delta = maxAngle;
        if (delta < -maxAngle) delta = -maxAngle;

        printf("Delta: %2.2f\n", delta * RAD2DEG);
        car.angle += delta;

        if (!CAR_move()) {
            car.angle += delta;
            CAR_move();
            CAR_findPath(path[pathLen-1].x, path[pathLen-1].y);
        }
        step++;
    }

    // if (step >= pathLen){
    //     if (car.position.x != path[pathLen-1].x || car.position.y != path[pathLen-1].y){
    //         CAR_findPath(path[pathLen-1].x, path[pathLen-1].y);
    //     }
    // }
    

    return step;
}

int CAR_findPath(uint x, uint y){
    point_t start = {.x = car.position.x, .y = car.position.y};
    point_t end = {.x = x, .y = y};
    point_t prefer = {.x = cosf(car.angle), y = sinf(car.angle)};

    pathLen = PATHFINDING_dijkstra(start, end, prefer, path);

    // if (pathLen){
    //     for (int i = 0; i < pathLen; i++){
    //         printf("%2i, %2i\n", path[i].x, path[i].y);
    //     }
    // }
    step = 0;
    return pathLen;
}