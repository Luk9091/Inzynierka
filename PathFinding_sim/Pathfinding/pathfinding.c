#include "pathfinding.h"

static point_t direction[] = {
    {0, -1}, // North
    // {1, -1}, // North East
    {1, 0},  // East
    // {1, 1},  // South East
    {0, 1},  // South
    // {-1, 1}, // South West
    {-1, 0}, // West
    // {-1, -1} // North West
};

static float calculateDistance(point_t a, point_t b){
    return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2));
}

static int calculateAngle(point_t a, point_t b){
    int angle = atan2f(b.y - a.y, b.x - a.x) * RAD2DEG;
    // if (angle < 0){
    //     angle += 360;
    // }
    return angle;
}

static int calculateAngle3(point_t a, point_t b, point_t c){
    int angle1 = calculateAngle(a, b);
    int angle2 = calculateAngle(b, c);
    return angle2 - angle1;
}

static bool isColinear(point_t a, point_t b, point_t c){
    return (b.y - a.y) * (c.x - b.x) == (c.y - b.y) * (b.x - a.x);
}

static bool isRightAngleTurn(point_t oldDir, point_t newDir){
    if (oldDir.x != newDir.x && oldDir.y != newDir.y)
    {
        return true;
    }

    return false;
}


static bool isOppositeDirection(point_t oldDir, point_t newDir){
    point_t direction = {
        .x = oldDir.x + newDir.x,
        .y = oldDir.y + newDir.y};
    if (direction.x == 0 && direction.y == 0)
    {
        printf("Old: %i, %i\n", oldDir.x, oldDir.y);
        printf("New: %i, %i\n", newDir.x, newDir.y);
        printf("Opposite direction\n");
        return true;
    }

    return false;
}


point_t calculatePointWithDistance(point_t start, float angle, int distance){
    return (point_t){
        .x = start.x + distance * cosf(angle * DEG2RAD),
        .y = start.y + distance * sinf(angle * DEG2RAD)};
}

static bool PATHFINDING_isValid(point_t current, point_t dir){
    point_t next = {
        .x = current.x + dir.x,
        .y = current.y + dir.y
    };

    if (
        current.x < 0 || current.y < 0 || current.x >= MAP_SIZE_X * CELL_SIZE_X || current.y >= MAP_SIZE_Y * CELL_SIZE_Y ||
        next.x < 0 || next.y < 0 || next.x >= MAP_SIZE_X * CELL_SIZE_X || next.y >= MAP_SIZE_Y * CELL_SIZE_Y
    ){
        return false;
    }

    if (dir.x != 0 && dir.y != 0){
        if (
            MAP_getType(current.x, next.y) == WALL ||
            MAP_getType(next.x, current.y) == WALL
        ){
            return false;
        }
    }

    cellType_t currentType = MAP_getType(current.x, current.y);
    cellType_t nextType = MAP_getType(next.x, next.y);

    if (nextType == NONE){
        return true;
    }

    if (nextType != ROAD){
        return false;
    }

    return true;
}

static void PATHFINDING_initNodes(node_t nodes[MAP_SIZE_X * CELL_SIZE_X][MAP_SIZE_Y * CELL_SIZE_Y]){
    for (int x = 0; x < MAP_SIZE_X * CELL_SIZE_X; x++){
        for (int y = 0; y < MAP_SIZE_Y * CELL_SIZE_Y; y++){
            nodes[x][y].distance = INF;
            nodes[x][y].prev = (point_t){-1, -1};
        }
    }
}

int PATHFINDING_dijkstra(point_t start, point_t destination, point_t preferredDirection, list_t *pathList){
    node_t nodes[MAP_SIZE_X * CELL_SIZE_X][MAP_SIZE_Y * CELL_SIZE_Y];
    bool visited[MAP_SIZE_X * CELL_SIZE_X][MAP_SIZE_Y * CELL_SIZE_Y] = {false};

    PATHFINDING_initNodes(nodes);

    nodes[start.x][start.y].distance = 0;

    for (int count = 0; count < MAP_SIZE_X * MAP_SIZE_Y * CELL_SIZE_X * CELL_SIZE_Y - 1; count++){
        int min = INF;
        point_t minPoint = {-1, -1};

        for (int x = 0; x < MAP_SIZE_X * CELL_SIZE_X; x++){
            for (int y = 0; y < MAP_SIZE_Y * CELL_SIZE_Y; y++){
                if (!visited[x][y] && nodes[x][y].distance <= min){
                    min = nodes[x][y].distance;
                    minPoint = (point_t){x, y};
                    preferredDirection = (point_t){x - nodes[x][y].prev.x, y - nodes[x][y].prev.y};
                }
            }
        }

        if (minPoint.x == -1 || minPoint.y == -1){
            break;
        }
        visited[minPoint.x][minPoint.y] = true;

        for (int i = 0; i < sizeof(direction) / sizeof(direction[0]); i++){
            if (PATHFINDING_isValid(minPoint, direction[i])){
                point_t new = {minPoint.x + direction[i].x, minPoint.y + direction[i].y};
                int distance = abs(direction[i].x) + abs(direction[i].y);
                if (preferredDirection.x != direction[i].x || preferredDirection.y != direction[i].y){
                    distance = distance * 4;
                    if (isRightAngleTurn(preferredDirection, direction[i])){
                        distance = distance * 4;
                    }
                }
                if (!visited[new.x][new.y] && nodes[minPoint.x][minPoint.y].distance + distance < nodes[new.x][new.y].distance){
                    nodes[new.x][new.y].distance = nodes[minPoint.x][minPoint.y].distance + distance;
                    nodes[new.x][new.y].prev = minPoint;
                }
            }
        }
    }

    if (nodes[destination.x][destination.y].distance == INF){
        return 0;
    }

    int pathLen = pathList->length;
    for (point_t at = destination; at.x != -1; at = nodes[at.x][at.y].prev){
        list_insert(pathList, &at, pathLen);
    }

    return pathList->length;
}

int PATHFINDING_fromPathToLinear(list_t *pathList, list_t *instructionList){
    int index = 0;
    if (pathList->length == 1){
        return 0;
    }

    instruction_t newInstruction;
    list_at(pathList, &(newInstruction.start), 0);
    newInstruction.distance = 0;
    newInstruction.angle = 0;
    newInstruction.isArc = false;
    newInstruction.center = (point_t){0, 0};
    list_push(instructionList, &newInstruction);

    instruction_t *instruction;
    point_t middle, next;
    for (int i = 1; i < pathList->length; i++){
        instruction = list_item(instructionList, index);
        list_at(pathList, &middle, i - 1);
        list_at(pathList, &next, i);
        if (!isColinear(instruction->start, middle, next)){
            instruction->distance = calculateDistance(instruction->start, middle) * DISTANCE_PER_PIXEL;
            instruction->angle = calculateAngle(instruction->start, middle);
            instruction->end = middle;
            index++;
            newInstruction.start = middle;
            newInstruction.distance = 0;
            newInstruction.angle = 0;
            list_push(instructionList, &newInstruction);
        }
    }

    // Final segment
    instruction = list_item(instructionList, index);
    instruction->angle = calculateAngle(newInstruction.start, next);
    if(instruction->angle < 0){
        instruction->distance = floorf(calculateDistance(newInstruction.start, next) * DISTANCE_PER_PIXEL);
    } else {
        instruction->distance = ceilf(calculateDistance(newInstruction.start, next) * DISTANCE_PER_PIXEL);
    }
    instruction->end = next;

    // return instructionIndex + 1;
    return instructionList->length;
}


point_t findCenter(instruction_t *prev, instruction_t *next, instruction_t *new){
    point_t center;
    int deltaAngle = next->angle + prev->angle + 90;
    new->arcAngle = deltaAngle - 90;

    point_t move = {
        ((float)new->radius * cosf((deltaAngle/2) * DEG2RAD)),
        ((float)new->radius * sinf((deltaAngle/2) * DEG2RAD)) 
    };
    

    center.x = prev->end.x - move.x;
    center.y = prev->end.y - move.y;


    return center;
}

bool createCircle(instruction_t *prev, instruction_t *next, instruction_t *new){
    line_t linePrev = GEOMETRY_lineEquation(prev->angle, prev->end);
    line_t lineNext = GEOMETRY_lineEquation(next->angle, next->start);
    float bisectorAngle = GEOMETRY_bisectorAngle(prev->angle, next->angle);
    float minRadius = (MIN_RADIUS / (float)DISTANCE_PER_PIXEL);
    float delta = -0.1f;


    if (prev->end.y != prev->start.y){
        if (next->end.x - next->start.x > 0){
            delta = -delta;
        } 
        // if (linePrev.A > 0) {
        //     bisectorAngle += 45;
        // } else {
        //     // bisectorAngle -= 45;
        // }
        bisectorAngle += 90;
    } else {
        if (prev->end.x - prev->start.x < 0){
            delta = -delta;
        }
    }
    float x = next->start.x;
    for (int i = 0; i < 100; i++){
        x += delta;
        point_t center = GEOMETRY_pointOnLine(bisectorAngle, prev->end, x);

        int distancePrev = round(GEOMETRY_distancePointToLine(linePrev, center));
        int distanceNext = round(GEOMETRY_distancePointToLine(lineNext, center));

        if(distancePrev >= minRadius && distanceNext >= minRadius){
            new->isArc = true;
            new->radius = distancePrev;
            new->center = center;

            new->start = GEOMETRY_tangentPoint(center, new->radius, linePrev);
            new->end   = GEOMETRY_tangentPoint(center, new->radius, lineNext);

            return true;
        }
    }
    
    return false;
}



void PATHFINDING_connectInstructionWithArc(list_t *instructionList){
    if (instructionList->length < 2){
        return;
    }


    instruction_t *instruction;
    instruction_t *nextInstruction;
    instruction_t newInstruction;

    for (int i = 0; i < instructionList->length - 1; i++){
        instruction = list_item(instructionList, i);
        nextInstruction = list_item(instructionList, i + 1);

        if (createCircle(instruction, nextInstruction, &newInstruction)){
            newInstruction.angle = calculateAngle(newInstruction.start, newInstruction.end);
            newInstruction.arcAngle = GEOMETRY_angleBetweenPoints(newInstruction.start, newInstruction.center, newInstruction.end) * RAD2DEG;
            newInstruction.distance = 2*π * (newInstruction.radius * DISTANCE_PER_PIXEL + CAR_PHYSICAL_WIDTH) * (abs(newInstruction.arcAngle)/360.f);
            newInstruction.angle = instruction->angle;
            list_insert(instructionList, &newInstruction, i+1);

            instruction->end = newInstruction.start;
            nextInstruction->start = newInstruction.end;

            instruction->distance = calculateDistance(instruction->start, instruction->end) * DISTANCE_PER_PIXEL;
            nextInstruction->distance = calculateDistance(nextInstruction->start, nextInstruction->end) * DISTANCE_PER_PIXEL;

            i++;
        }
    }

    PATHFINDING_removeEmptyInstructions(instructionList);
}



void PATHFINDING_removeEmptyInstructions(list_t *instructionList){
    uint listSize = instructionList->length;
    
    for (int i = listSize - 1; i > 0; i--){
        instruction_t *instruction = list_item(instructionList, i);
        if (instruction->distance == 0){
            list_removeAt(instructionList, i);
        }
    }
}



int PATHFINDING_fromPathToInstruction(list_t *pathList, list_t *instructionList){
    // int linesLen = PATHFINDING_fromPathToLinear(path, pathLen, instruction);
    // return linesLen;
    return 0;
}