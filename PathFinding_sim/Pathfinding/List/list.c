#include "list.h"
#include "printf_color.h"


int list_init(list_t *list, size_t element_size){
    list->first = NULL;
    list->last = NULL;
    list->current = NULL;
    list->length = 0;
    list->element_size = element_size;
    return LIST_ERROR_OK;
}

int list_free(list_t *list){
    _list_node_t *current = list->first;
    while (current != NULL){
        list->length--;
        _list_node_t *next = (_list_node_t*)current->next;
        free(current->data);
        free(current);
        current = next;
    }
    list->first = NULL;
    list->last = NULL;
    list->current = NULL;

    return LIST_ERROR_OK;
}


int list_push(list_t *list, void *data){
    _list_node_t* newNode = (_list_node_t*)malloc(sizeof(_list_node_t));
    newNode->data = (uint8_t*)malloc(list->element_size);


    if (newNode == NULL){
        return LIST_ALLOC_ERROR;
    }

    memcpy(newNode->data, data, list->element_size);
    newNode->next = NULL;

    if (list->first == NULL){
        list->first = newNode;
    }
    
    if (list->last != NULL){
        list->last->next = (void*)newNode;
    }
    newNode->prev = (void*)list->last;

    list->last = newNode;

    list->length++;
    return LIST_ERROR_OK;
}


int list_pop(list_t *list, void *data){
    if (list->length == 0){
        return LIST_ERROR_EMPTY;
    }

    _list_node_t *last = list->last;
    memcpy(data, last->data, list->element_size);
    list->last = (_list_node_t*)last->prev;
    list->last->next = NULL;

    free(last->data);
    free(last);
    list->length--;
    
    return LIST_ERROR_OK;
}


int list_at(list_t *list, void *data, size_t index){
    if (index >= list->length){
        return LIST_INDEX_ERROR;
    } else if (list->length == 0){
        return LIST_ERROR_EMPTY;
    }
    

    _list_node_t *current = list->first;
    for (int i = 0; i < index; i++){
        current = (_list_node_t*)current->next;
    }

    memcpy(data, current->data, list->element_size);
    return LIST_ERROR_OK;
}

void *list_item(list_t *list, size_t index){
    if (index >= list->length){
        return NULL;
    } else if (list->length == 0){
        return NULL;
    }
    

    _list_node_t *current = list->first;
    for (int i = 0; i < index; i++){
        current = (_list_node_t*)current->next;
    }

    return current->data;
}

int list_removeAt(list_t *list, size_t index){
    if (index >= list->length){
        return LIST_INDEX_ERROR;
    } else if (list->length == 0){
        return LIST_ERROR_EMPTY;
    }

    _list_node_t *current = list->first;
    for (int i = 0; i < index; i++){
        current = (_list_node_t*)current->next;
    }

    ((_list_node_t*)current->prev)->next = current->next;
    ((_list_node_t*)current->next)->prev = current->prev;

    free(current->data);
    free(current);
    list->length--;
    return LIST_ERROR_OK;
}


int list_popAt(list_t *list, void *data, size_t index){
    int status = list_at(list, data, index);
    if (status != LIST_ERROR_OK){
        return status;
    }
    status = list_removeAt(list, index);
    return status;
}

int list_insert(list_t *list, void *data, size_t index){
    if (index > list->length){
        return LIST_INDEX_ERROR;
    } 

    if (index == list->length){
        return list_push(list, data);
    }

    _list_node_t *current = list->first;
    for (int i = 0; i < index; i++){
        current = (_list_node_t*)current->next;
    }

    _list_node_t* newNode = (_list_node_t*)malloc(sizeof(_list_node_t));
    newNode->data = (uint8_t*)malloc(list->element_size);
    memcpy(newNode->data, data, list->element_size);

    newNode->next = current;
    newNode->prev = current->prev;

    if (index == 0){
        list->first = newNode;
    } else {
        ((_list_node_t*)current->prev)->next = (void*)newNode;
    }
    current->prev = (void*)newNode;


    list->length++;
    return LIST_ERROR_OK;
}


int list_next(list_t * list, void *data){
    if (list->current == NULL){
        if (list->length == 0){
            return LIST_ERROR_EMPTY;
        }

        list->current = list->first;
    } else {
        list->current = (_list_node_t*)list->current->next;
    }

    memcpy(data, list->current->data, list->element_size);
    return LIST_ERROR_OK;
}




void printPointList(list_t *list){
    printf("##############################\n");
    printf("Point list\n");
    printf("List length: %li\n", list->length);
    for (uint i = 0; i < list->length; i++){
        point_t *point = list_item(list, i);
        printf("%3i Point: x: %3i, y: %3i\n", i, point->x, point->y);
    }
    printf("##############################\n");
}


void printInstructionList(list_t *list){
    printf("##############################\n");
    printf("Instruction list\n");
    printf("List length: %li\n", list->length);
    for (uint i = 0; i < list->length; i++){
        instruction_t *instruction = list_item(list, i);
        printf("%3i :\n", i);
        printf("\tstart: x: %3i, y: %3i\n", instruction->start.x, instruction->start.y);
        printf("\tend:   x: %3i, y: %3i\n", instruction->end.x, instruction->end.y);
        printf("\tdistance: %i\n", instruction->distance);
        printf("\tangle:    %i\n", instruction->angle);
        if (instruction->isArc){
            printf(T_GREEN"Arc:\n"T_RESET);
            printf("\tCenter: %i, %i\n", instruction->center.x, instruction->center.y);
            printf("\tRadius: %i\n", instruction->radius);
            printf("\tBend: %i\n", instruction->arcAngle);
        }
    }

    printf("##############################\n");
}