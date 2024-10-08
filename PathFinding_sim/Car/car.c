#include "car.h"
#include <time.h>

static car_t car;
static point_t path[MAP_SIZE_X * MAP_SIZE_Y * CELL_SIZE_X * CELL_SIZE_Y] = {0};

static uint step;
static int pathLen;
static float beamDistance[3] = {0};
static double path_time = 0;

void _CAR_moveTriangle(){
    Vector2 center = {
        .x  = car.position.x * PIXEL_SIZE_X - cosf(car.angle) * CELL_SIZE_X + MAP_OFFSET_X,
        .y  = car.position.y * PIXEL_SIZE_Y - sinf(car.angle) * CELL_SIZE_Y + MAP_OFFSET_Y
    };

    car.top.x = center.x + cosf(car.angle) * CAR_SIZE_X;
    car.top.y = center.y + sinf(car.angle) * CAR_SIZE_Y;

    car.left.x = center.x + cosf(car.angle + M_PI_2) * CAR_SIZE_X/2; 
    car.left.y = center.y + sinf(car.angle + M_PI_2) * CAR_SIZE_Y/2;

    car.right.x = center.x + cosf(car.angle - M_PI_2) * CAR_SIZE_X/2;
    car.right.y = center.y + sinf(car.angle - M_PI_2) * CAR_SIZE_Y/2;

}

//! Dont work
// Circle shouldn't move when car is continuos 
void CAR_drawTurnCircle(){
    if (getAngle() == 90) return;
    float angle = (float)(getAngle()-90) * DEG2RAD;
    point_t center;
    float r = CAR_PHYSICAL_LEN / sin(angle);

    if (getAngle() < 90){
        center.x = car.top.x + r * sin(angle);
        center.y = car.top.y - r * cos(angle);
    } else {
        center.x = car.top.x - r * sin(angle);
        center.y = car.top.y + r * cos(angle);
    }

    DrawCircleLines(center.x, center.y, r, WHITE);
}


void _CAR_drawMessage(const char *str){
    const static int fontSize = 20;
    static int height = fontSize/2;
    // int width = screenWidth - MAP_SIZE_X*CELL_SIZE_X * 7/PIXEL_SIZE_X;
    int width = screenWidth * 8/10;

    DrawText(str, width, height, fontSize, WHITE);
}

// void _CAR_drawMessage(const char *str, int line){
//     const static int fontSize = 20;
//     int height = line * fontSize + (line + 1) * (fontSize/4);
//     int width = screenHeight * 8/10;
// }

void _CAR_drawPath(){
    for (int i = step; i < pathLen; i += 1){
        POINT_draw(path[i].x, path[i].y, 0.5f, 0.5f, PIXEL_SIZE_X/4, ORANGE);
    }
}


void CAR_init(float x, float y, float direction){
    car.position.x = x;
    car.position.y = y;
    car.color = BLUE;

    CAR_setAngle(direction);
}

void CAR_setBeamDistance(float left, float center, float right){
    const static float mux = (float)(PIXEL_SIZE_X) / (float)(DISTANCE_PER_PIXEL);
    beamDistance[0] = left   * mux;
    beamDistance[1] = center * mux;
    beamDistance[2] = right  * mux;

    for (int i = 0; i < 3; i++){
        if (beamDistance[i] < 0)
            beamDistance[i] = 0;
    }
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
    CAR_drawBeam(-30, beamDistance[0]);
    CAR_drawBeam(  0, beamDistance[1]);
    CAR_drawBeam( 30, beamDistance[2]);

    // CAR_drawTurnCircle();

    
    const char *str = TextFormat("Car pos: %2.2f, %2.2f\nAngle: %2.2f\nDestination: %2i, %2i\nPathfinding time: %.4f", car.position.x, car.position.y, car.angle * RAD2DEG, path[pathLen-1].x, path[pathLen-1].y, path_time);
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
    if (angle >= 360){
        angle -= 360;
    } else if (angle < 0){
        angle += 360;
    }
    car.angle = angle * DEG2RAD;
    _CAR_moveTriangle();
}

float CAR_getAngle(){
    return car.angle;
}

void CAR_changeAngle(float angle){
    CAR_setBeamDistance(0, 0, 0);
    CAR_setAngle((car.angle * RAD2DEG) + angle);
}

void CAR_setPosition(int x, int y){
    car.position.x = x;
    car.position.y = y;
    _CAR_moveTriangle();
}

Vector2 CAR_getPosition(){
    return car.position;
}

bool CAR_move(){
    if (!calculateSpeed()) return false;
    bool collision = false;
    float deltaAngle = abs(getAngle() - 90) * DEG2RAD;
    
    Vector2 move = {
        .x = rounds(cos(deltaAngle)*cosf(car.angle), 2),
        .y = rounds(cos(deltaAngle)*sinf(car.angle), 2)
    };
    Vector2 newPos = {
        .x = car.position.x + move.x,
        .y = car.position.y + move.y
    };

    if (MAP_collisionDetect(newPos.x, car.position.y) || !(car.position.x + move.x >= 0 && car.position.x + move.x < (MAP_SIZE_X*CELL_SIZE_X))){
        newPos.x = roundf(car.position.x) - move.x/100;
        collision = true;
    } 
    if (MAP_collisionDetect(car.position.x, newPos.y) || !(car.position.y + move.y >= 0 && car.position.y + move.y < (MAP_SIZE_Y*CELL_SIZE_Y))){
        newPos.y = roundf(car.position.y) - move.y/100;
        collision = true;
    } 


    car.position = newPos;
    CAR_changeAngle((90 - getAngle())/2);
    
    CAR_setBeamDistance(0, 0, 0);
    _CAR_moveTriangle();
    return !collision;
}

bool CAR_moveBackward(){
    if (!calculateSpeed()) return false;
    bool collision = false;
    float deltaAngle = abs(getAngle() - 90) * DEG2RAD;
    
    Vector2 move = {
        .x = - rounds(cos(deltaAngle)*cos(car.angle), 2),
        .y = - rounds(cos(deltaAngle)*sin(car.angle), 2)
    };
    Vector2 newPos = {
        .x = car.position.x + move.x,
        .y = car.position.y + move.y
    };

    if (MAP_collisionDetect(newPos.x, car.position.y) || !(car.position.x + move.x >= 0 && car.position.x + move.x < (MAP_SIZE_X*CELL_SIZE_X))){
        newPos.x = roundf(car.position.x) - move.x/100;
        collision = true;
    } 
    if (MAP_collisionDetect(car.position.x, newPos.y) || !(car.position.y + move.y >= 0 && car.position.y + move.y < (MAP_SIZE_Y*CELL_SIZE_Y))){
        newPos.y = roundf(car.position.y) - move.y/100;
        collision = true;
    } 


    car.position = newPos;
    CAR_changeAngle((90 - getAngle())/2);

    CAR_setBeamDistance(0, 0, 0);
    _CAR_moveTriangle();
    return !collision;
}

uint CAR_moveByPath(){
    const static float maxAngle = 20 * DEG2RAD;
    if (step < pathLen){
        int x = (int)(path[step].x) - (int)(car.position.x);
        int y = (int)(path[step].y) - (int)(car.position.y);
        float newAngle = rounds(atan2f(y, x), 2);
        
        static float oldDelta = 0;
        float delta = newAngle - car.angle;

        if (delta > π) {delta -= 2*π;}
        if (delta < -π){delta += 2*π;}


        if (delta > maxAngle) delta = maxAngle;
        if (delta < -maxAngle) delta = -maxAngle;

        // printf("Delta: %2.2f\n", delta * RAD2DEG);
        car.angle += delta;
        if ((oldDelta - delta)*RAD2DEG > 10){
            setDeltaAngle(delta*RAD2DEG);
        } else {
            setAngle(90);
        }
        oldDelta = delta;

        CAR_move();
        // if (!CAR_move()) {
        //     // car.angle += delta;
        //     CAR_move();
        //     CAR_findPath(path[pathLen-1].x, path[pathLen-1].y);
        // }
        step++;
        return true;
    }
    return false;
}

int CAR_findPath(uint x, uint y){
    point_t start = {.x = car.position.x, .y = car.position.y};
    point_t end = {.x = x, .y = y};
    point_t prefer = {.x = ceilf(cosf(car.angle)), y = ceilf(sinf(car.angle))};

    clock_t t_start = clock();
    pathLen = PATHFINDING_dijkstra(start, end, prefer, path);
    clock_t t_end = clock();
    path_time = (double)(t_end - t_start) / CLOCKS_PER_SEC;

    step = 1;
    return pathLen;
}

int CAR_addPath(uint x, uint y){
    point_t start = path[pathLen-1];
    point_t end = {x, y};
    point_t prefer = {
        .x = path[pathLen - 1].x - path[pathLen - 2].x,
        .y = path[pathLen - 1].y - path[pathLen - 2].y,
    };

    clock_t t_start = clock();
    pathLen += PATHFINDING_dijkstra(start, end, prefer, path + pathLen - 1) - 1;
    clock_t t_end = clock();
    path_time = (double)(t_end - t_start) / CLOCKS_PER_SEC;

    return pathLen;
}