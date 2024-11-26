#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <memory.h>

#include <stdio.h>
#include "point.h"

#define LIST_ERROR_OK     0
#define LIST_ERROR_EMPTY -1
#define LIST_ALLOC_ERROR -2
#define LIST_INDEX_ERROR -3
#define LIST_EOL_ERROR   -4 // End of list error


typedef struct _list_node{
    uint8_t *data;
    void *next;
    void *prev;
} _list_node_t;

typedef struct _list{
    _list_node_t *first;
    _list_node_t *last;
    _list_node_t *current;
    size_t length;
    size_t element_size;
} list_t;

int list_init(list_t *list, size_t element_size);
int list_free(list_t *list);

int list_push(list_t *list, void *data);
int list_pop(list_t *list, void *data);
int list_at(list_t *list, void *data, size_t index);
void *list_item(list_t *list, size_t index);
int list_removeAt(list_t *list, size_t index);
int list_popAt(list_t *list, void *data, size_t index);
int list_insert(list_t *list, void *data, size_t index);

int list_next(list_t *list, void *data);

void printPointList(list_t *list);
void printInstructionList(list_t *list);

#endif // __LIST_H__