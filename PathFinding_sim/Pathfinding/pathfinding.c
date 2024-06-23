#include "pathfinding.h"

static void NODE_init(node_t *node, uint x, uint y){
    node->x = x;
    node->y = y;
    node->children = 0;
    node->next = NULL;
}

static void NODE_push(node_t *node, node_t *next, int distance){
    if (node->children == 0){
        node->next = malloc(sizeof(node_t));
    } else{
        node->next = realloc(node->next, sizeof(node_t)*(node->children+1));
    }

    
    node->next[node->children].node = next;
    node->next[node->children].distance = distance;
    node->children++;
}


void PATHFINDING_init(uint start_x, uint start_y, uint end_x, uint end_y){
    node_t start;
    node_t next;
    NODE_init(&start, start_x, start_y);
    NODE_init(&next, 0, 1);
    NODE_push(&start, &next, 1);

}