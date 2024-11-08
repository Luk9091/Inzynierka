#include "pathfinding.h"

point_t direction[] = {
    {0, -1}, // North
    {1, -1}, // North East
    {1, 0},  // East
    {1, 1},  // South East
    {0, 1},  // South
    {-1, 1}, // South West
    {-1, 0}, // West
    {-1, -1} // North West
};

static float calculateDistance(point_t a, point_t b)
{
    return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2));
}

static int calculateAngle(point_t a, point_t b)
{
    return atan2f(b.y - a.y, b.x - a.x) * RAD2DEG;
}

static bool isColinear3(point_t a, point_t b, point_t c)
{
    return (b.y - a.y) * (c.x - b.x) == (c.y - b.y) * (b.x - a.x);
}

bool isRightAngleTurn(point_t oldDir, point_t newDir)
{
    if (oldDir.x != newDir.x && oldDir.y != newDir.y)
    {
        return true;
    }

    return false;
}

bool isOppositeDirection(point_t oldDir, point_t newDir)
{
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

bool connectWithArc(point_t a, point_t b, point_t *center, int *r)
{
    float distance = calculateDistance(a, b);

    if (distance < 2 * MIN_RADIUS)
    {
        return false;
    }

    *r = fminf(fmaxf(distance / 2, MIN_RADIUS), MIN_RADIUS);
    center->x = (a.x + b.x) / 2;
    center->y = (a.y + b.y) / 2;
    return true;
}

point_t calculatePointWithDistance(point_t start, float angle, int distance)
{
    return (point_t){
        .x = start.x + distance * cosf(angle * DEG2RAD),
        .y = start.y + distance * sinf(angle * DEG2RAD)};
}

bool PATHFINDING_isValid(point_t current, point_t dir)
{
    point_t next = {
        .x = current.x + dir.x,
        .y = current.y + dir.y};

    if (
        current.x < 0 || current.y < 0 || current.x >= MAP_SIZE_X * CELL_SIZE_X || current.y >= MAP_SIZE_Y * CELL_SIZE_Y ||
        next.x < 0 || next.y < 0 || next.x >= MAP_SIZE_X * CELL_SIZE_X || next.y >= MAP_SIZE_Y * CELL_SIZE_Y)
    {
        return false;
    }

    if (dir.x != 0 && dir.y != 0)
    {
        if (
            MAP_getType(current.x, next.y) == WALL ||
            MAP_getType(next.x, current.y) == WALL)
        {
            return false;
        }
    }

    cellType_t currentType = MAP_getType(current.x, current.y);
    cellType_t nextType = MAP_getType(next.x, next.y);

    if (nextType == NONE)
    {
        return true;
    }

    if (nextType != ROAD)
    {
        return false;
    }

    return true;
}

void PATHFINDING_initNodes(node_t nodes[MAP_SIZE_X * CELL_SIZE_X][MAP_SIZE_Y * CELL_SIZE_Y])
{
    for (int x = 0; x < MAP_SIZE_X * CELL_SIZE_X; x++)
    {
        for (int y = 0; y < MAP_SIZE_Y * CELL_SIZE_Y; y++)
        {
            nodes[x][y].distance = INF;
            nodes[x][y].prev = (point_t){-1, -1};
        }
    }
}

int PATHFINDING_dijkstra(point_t start, point_t destination, point_t preferredDirection, list_t *pathList)
{
    node_t nodes[MAP_SIZE_X * CELL_SIZE_X][MAP_SIZE_Y * CELL_SIZE_Y];
    bool visited[MAP_SIZE_X * CELL_SIZE_X][MAP_SIZE_Y * CELL_SIZE_Y] = {false};

    PATHFINDING_initNodes(nodes);

    nodes[start.x][start.y].distance = 0;

    for (int count = 0; count < MAP_SIZE_X * MAP_SIZE_Y * CELL_SIZE_X * CELL_SIZE_Y - 1; count++)
    {
        int min = INF;
        point_t minPoint = {-1, -1};

        for (int x = 0; x < MAP_SIZE_X * CELL_SIZE_X; x++)
        {
            for (int y = 0; y < MAP_SIZE_Y * CELL_SIZE_Y; y++)
            {
                if (!visited[x][y] && nodes[x][y].distance <= min)
                {
                    min = nodes[x][y].distance;
                    minPoint = (point_t){x, y};
                    preferredDirection = (point_t){x - nodes[x][y].prev.x, y - nodes[x][y].prev.y};
                }
            }
        }

        if (minPoint.x == -1 || minPoint.y == -1)
        {
            break;
        }
        visited[minPoint.x][minPoint.y] = true;

        for (int i = 0; i < sizeof(direction) / sizeof(direction[0]); i++)
        {
            if (PATHFINDING_isValid(minPoint, direction[i]))
            {
                point_t new = {minPoint.x + direction[i].x, minPoint.y + direction[i].y};
                int distance = abs(direction[i].x) + abs(direction[i].y);
                if (preferredDirection.x != direction[i].x || preferredDirection.y != direction[i].y)
                {
                    distance = distance * 4;
                    if (isRightAngleTurn(preferredDirection, direction[i]))
                    {
                        distance = distance * 4;
                    }
                }
                if (!visited[new.x][new.y] && nodes[minPoint.x][minPoint.y].distance + distance < nodes[new.x][new.y].distance)
                {
                    nodes[new.x][new.y].distance = nodes[minPoint.x][minPoint.y].distance + distance;
                    nodes[new.x][new.y].prev = minPoint;
                }
            }
        }
    }

    if (nodes[destination.x][destination.y].distance == INF)
    {
        return 0;
    }

    int pathLen = pathList->length;
    for (point_t at = destination; at.x != -1; at = nodes[at.x][at.y].prev){
        list_insert(pathList, &at, pathLen);
    }

    printPointList(pathList);
    return pathList->length;
}

int PATHFINDING_fromPathToLinear(list_t *pathList, list_t *instructionList){
    int index = 0;

    instruction_t newInstruction;
    list_at(pathList, &(newInstruction.start), 0);
    newInstruction.distance = 0;
    newInstruction.angle = 0;
    list_push(instructionList, &newInstruction);

    instruction_t *instruction;
    point_t middle, next;
    for (int i = 1; i < pathList->length; i++)
    {
        instruction = list_item(instructionList, index);
        list_at(pathList, &middle, i - 1);
        list_at(pathList, &next, i);
        if (!isColinear3(instruction->start, middle, next))
        {
            instruction->distance = calculateDistance(instruction->start, middle);
            instruction->angle = calculateAngle(instruction->start, middle);
            index++;
            newInstruction.start = middle;
            newInstruction.distance = 0;
            newInstruction.angle = 0;
            list_push(instructionList, &newInstruction);
        }
    }

    // Final segment
    instruction->angle = calculateAngle(newInstruction.start, next);
    if(instruction->angle < 0){
        instruction->distance = floorf(calculateDistance(newInstruction.start, next));
    } else {
        instruction->distance = ceilf(calculateDistance(newInstruction.start, next));
    }

    // return instructionIndex + 1;
    printInstructionList(instructionList);
    return instructionList->length;
}



int PATHFINDING_fromPathToInstruction(list_t *pathList, list_t *instructionList)
{
    // int linesLen = PATHFINDING_fromPathToLinear(path, pathLen, instruction);
    // return linesLen;
    return 0;
}