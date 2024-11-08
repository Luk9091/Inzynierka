#include "car.h"
#include <time.h>

static car_t car;

static uint step;
static int beamDistance[3] = {0};
const static float beam_mux = (float)(PIXEL_SIZE_X) / (float)(DISTANCE_PER_PIXEL);
static double path_time = 0;
static int deltaAngel = 0;


static list_t path_list;
static list_t instruction_list;

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

void _CAR_drawPath(){
    for (int i = step; i < path_list.length; i++){
        point_t point;
        list_at(&path_list, &point, i);
        POINT_draw(point.x, point.y, 0.5f, 0.5f, PIXEL_SIZE_X/4, ORANGE);
    }
}

void _CAR_drawInstruction(){
    for (int i = 0; i < instruction_list.length; i++){
        instruction_t *instruction = list_item(&instruction_list, i);
        Vector2 endPoint = {
            .x = instruction->start.x + instruction->distance * cosf(instruction->angle * DEG2RAD),
            .y = instruction->start.y + instruction->distance * sinf(instruction->angle * DEG2RAD)
        };

        Vector2 start = {
            .x = instruction->start.x,
            .y = instruction->start.y 
        };


        DrawLine(
            start.x * PIXEL_SIZE_X + MAP_OFFSET_X,
            start.y * PIXEL_SIZE_Y + MAP_OFFSET_Y,
            endPoint.x * PIXEL_SIZE_X + MAP_OFFSET_X,
            endPoint.y * PIXEL_SIZE_Y + MAP_OFFSET_Y,
            RED
        );
    }
}


void CAR_init(float x, float y, float direction){
    car.position.x = x;
    car.position.y = y;
    car.color = BLUE;

    CAR_setAngle(direction);
    CAR_setBeamDistance(0, 0, 0);

    list_init(&path_list, sizeof(point_t));
    list_init(&instruction_list, sizeof(instruction_t));
}

void CAR_setBeamDistance(int left, int center, int right){
    beamDistance[0] = left  ;
    beamDistance[1] = center;
    beamDistance[2] = right ;

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
    _CAR_drawInstruction();
    // _CAR_drawTurnCircle();


    DrawTriangle(
        car.left, car.top, car.right,
        RED
    );
    CAR_drawBeam(-30, beamDistance[0] * beam_mux);
    CAR_drawBeam(  0, beamDistance[1] * beam_mux);
    CAR_drawBeam( 30, beamDistance[2] * beam_mux);

    
    GUI_print(TextFormat("Car pos: %2.2f, %2.2f", car.position.x, car.position.y), -1);
    GUI_print(TextFormat("Angle: %2.2f", car.angle * RAD2DEG), -1);
    point_t *destination = list_item(&path_list, path_list.length - 1);
    if (destination != NULL)
        GUI_print(TextFormat("Destination: %2i, %2i", destination->x, destination->y), -1);
    else
        GUI_print(TextFormat("Destination: %2i, %2i", -1, -1), -1);
    GUI_print(TextFormat("Time: %.4f", path_time), -1);
    GUI_print(TextFormat("Distance %3i, %3i, %3i", beamDistance[0], beamDistance[1], beamDistance[2]), -1);
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

void CAR_setPosition(float x, float y){
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
    // float deltaAngle = (90 - getAngle()) * DEG2RAD;
    
    Vector2 move = {
        .x = cos(car.angle),
        .y = sin(car.angle)
    };
    Vector2 newPos = {
        .x = car.position.x + move.x,
        .y = car.position.y + move.y,
    };
    newPos.x = rounds(newPos.x, 2);
    newPos.y = rounds(newPos.y, 2);

    if (MAP_collisionDetect(newPos.x, car.position.y) || !(car.position.x + move.x >= 0 && car.position.x + move.x < (MAP_SIZE_X*CELL_SIZE_X))){
        if (move.x > 0){
            newPos.x = floorf(newPos.x) - move.x/100;
        } else {
            newPos.x = ceilf(newPos.x) - move.x/100;
        }
        collision = true;
    } 
    if (MAP_collisionDetect(car.position.x, newPos.y) || !(car.position.y + move.y >= 0 && car.position.y + move.y < (MAP_SIZE_Y*CELL_SIZE_Y))){
        if (move.y > 0){
            newPos.y = floorf(newPos.y) - move.y/100;
        } else {
            newPos.y = ceilf(newPos.y) - move.y/100;
        }
        collision = true;
    } 

    car.position = newPos;
    CAR_changeAngle((getAngle() - 90)/3);
    _CAR_moveTriangle();
    return !collision;
}

bool CAR_moveBackward() {
    if (!calculateSpeed()) return false;
    bool collision = false;
    float deltaAngle = (90 - getAngle())/3.f * DEG2RAD;
    
    Vector2 move = {
        .x = -cos(car.angle + deltaAngle),
        .y = -sin(car.angle + deltaAngle)
    };
    Vector2 newPos = {
        .x = car.position.x + move.x,
        .y = car.position.y + move.y,
    };
    newPos.x = rounds(newPos.x, 2);
    newPos.y = rounds(newPos.y, 2);

    if (MAP_collisionDetect(newPos.x, car.position.y) || !(car.position.x + move.x >= 0 && car.position.x + move.x < (MAP_SIZE_X * CELL_SIZE_X))) {
        if (move.x > 0) {
            newPos.x = floorf(newPos.x) - move.x / 100;
        } else {
            newPos.x = ceilf(newPos.x) - move.x / 100;
        }
        collision = true;
    }
    if (MAP_collisionDetect(car.position.x, newPos.y) || !(car.position.y + move.y >= 0 && car.position.y + move.y < (MAP_SIZE_Y * CELL_SIZE_Y))) {
        if (move.y > 0) {
            newPos.y = floorf(newPos.y) - move.y / 100;
        } else {
            newPos.y = ceilf(newPos.y) - move.y / 100;
        }
        collision = true;
    }

    car.position = newPos;
    CAR_changeAngle(-(getAngle() - 90)/3);
    _CAR_moveTriangle();
    return !collision;
}

uint CAR_moveByPath(){
    const float maxAngle = 30 * DEG2RAD;
    static float oldDelta = 0;
    if (step < path_list.length){
        point_t *path = list_item(&path_list, step);
        int x = (int)(path->x) - (int)(car.position.x);
        int y = (int)(path->y) - (int)(car.position.y);
        float newAngle = rounds(atan2f(y, x), 2);
        
        float delta = car.angle - newAngle;

        if (delta > π) {delta -= 2*π;}
        if (delta < -π){delta += 2*π;}


        if (delta > maxAngle) delta = maxAngle;
        if (delta < -maxAngle) delta = -maxAngle;

        setAngle(90 - delta*RAD2DEG);
        oldDelta = delta;

        CAR_move();
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
    list_free(&path_list);
    list_free(&instruction_list);
    PATHFINDING_dijkstra(start, end, prefer, &path_list);
    PATHFINDING_fromPathToLinear(&path_list, &instruction_list);

    clock_t t_end = clock();
    path_time = (double)(t_end - t_start) / CLOCKS_PER_SEC;

    step = 1;
    return path_list.length;
}

int CAR_addPath(uint x, uint y){
    point_t start; list_pop(&path_list, &start);
    point_t end = {x, y};
    point_t *toPrefer = list_item(&path_list, path_list.length - 2);
    point_t prefer = {
        .x = start.x - toPrefer->x,
        .y = start.y - toPrefer->y,
    };

    clock_t t_start = clock();
    list_free(&instruction_list);
    
    int steps = PATHFINDING_dijkstra(start, end, prefer, &path_list);
    PATHFINDING_fromPathToLinear(&path_list, &instruction_list);
    clock_t t_end = clock();
    path_time = (double)(t_end - t_start) / CLOCKS_PER_SEC;

    return steps;
}



void CAR_free(){
    list_free(&path_list);
    list_free(&instruction_list);
}