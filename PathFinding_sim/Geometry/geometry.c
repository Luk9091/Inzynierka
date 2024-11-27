#include "pathfinding.h"


point_t GEOMETRY_pointOnLine(float angle, point_t point, float x){
    point_t result = {
        .x = round(x),
        .y = point.y + round((tanf(angle * DEG2RAD) * (x - (float)point.x)))
    };

    if (abs(angle) > 90){
        result.x = x + 1;
    }
    return result;
}


line_t GEOMETRY_lineEquation(float angle, point_t linePoint){
    line_t line;

    if (abs(angle) % 180 == 90){
        line = (line_t){
            .A = -1,
            .B = 0,
            .C = linePoint.x,
        };
    } else if (abs(angle)%180 == 0){
        line = (line_t){
            .A = 0,
            .B = 1,
            .C = -linePoint.y,
        };
    } else{
        angle = angle * DEG2RAD;
        line = (line_t){
            .A = -tanf(angle),
            .B = 1,
        };
        line.C = -linePoint.y -  line.A * linePoint.x;
    }
    
    return line;
}


point_t GEOMETRY_pointWithLine(line_t line, int x){
    point_t point = {
        .x = x,
        .y = (-line.A * x - line.C) / line.B,
    };
    return point;
}


float GEOMETRY_distancePointToLine(line_t line, point_t toPoint){
    float numerator = fabsf(line.A*toPoint.x + line.B*toPoint.y + line.C);
    float denominator = sqrtf(line.A*line.A + line.B*line.B);
    float distance = numerator / denominator;
    return distance;
}


float GEOMETRY_bisectorAngle(float angle1, float angle2){
    float ret1 = (180 - angle1 + angle2)/2;
    return ret1;
}

point_t GEOMETRY_closestPointOnLine(line_t line, point_t point){
    int x, y;
    if (line.A == 0){
        x = point.x;
        y = -line.C / line.B;
    } else if (line.B == 0){
        x = -line.C / line.A;
        y = point.y;
    } else {
        float d = line.A * point.x + line.B * point.y + line.C;
        x = point.x - line.A * d / (line.A * line.A + line.B * line.B);
        y = point.y - line.B * d / (line.A * line.A + line.B * line.B);
    }
    return (point_t){x, y};
}


point_t GEOMETRY_tangentPoint(point_t center, int radius, line_t line){
    point_t closest = GEOMETRY_closestPointOnLine(line, center);
    if (line.A == 0 || line.B == 0){
        return (point_t){closest.x, closest.y};
    }
    float normalVectorLen = sqrtf(line.A * line.A + line.B * line.B);
    point_t normalVector = (point_t){line.A / normalVectorLen, line.B / normalVectorLen};

    point_t tangent = (point_t){
        .x = closest.x + normalVector.x * radius,
        .y = closest.y + normalVector.y * radius
    };

    return tangent;
}


point_t GEOMETRY_vectorFromPoint(point_t a, point_t b){
    return (point_t){
        .x = b.x - a.x,
        .y = b.y - a.y,
    };
}

float GEOMETRY_dotProduct(point_t v1, point_t v2){
    return v1.x * v2.x + v1.y * v2.y;
}

float GEOMETRY_magnitude(point_t vector){
    return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

int GEOMETRY_normalVector(point_t v1, point_t v2){
    return v1.x * v2.y - v1.y * v2.x;
}

float GEOMETRY_angleBetweenPoints(point_t a, point_t b, point_t c){
    point_t ab = GEOMETRY_vectorFromPoint(a, b);
    point_t bc = GEOMETRY_vectorFromPoint(b, c);

    float dot = GEOMETRY_dotProduct(ab, bc);
    float mag = GEOMETRY_magnitude(ab) * GEOMETRY_magnitude(bc);
    float angle = acosf(dot / mag);
    int normal = GEOMETRY_normalVector(ab, bc);

    angle = sign(normal) * angle;

    return angle;
}