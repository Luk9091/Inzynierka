#include "queue.h"
#include <stdlib.h>
#include <memory.h>

void queue_init(queue_t *q, uint element_size, uint size){
    q->data = (uint8_t *)calloc(size + 1, element_size);
    q->element_size = element_size;
    q->size = size;
    q->length = 0;
    
    q->head = 0;
    q->tail = 0;

    pthread_mutex_t *mutex = malloc(sizeof(*mutex));
    q->mutex = mutex;
}


void queue_free(queue_t *q){
    free(q->data);
    free(q->mutex);
}

int queue_push(queue_t *q, void *data){
    if (q->length == q->size){
        return 1;
    }

    pthread_mutex_lock(q->mutex);
    memcpy(q->data + q->head*q->element_size, data, q->element_size);
    q->head++;
    q->length++;
    if (q->head >= q->size){
        q->head = 0;
    }
    pthread_mutex_unlock(q->mutex);
    return 0;
}

int queue_pop(queue_t *q, void *data){
    if (q->length == 0){
        return 1;
    }
    pthread_mutex_lock(q->mutex);
    memcpy(data, q->data + q->tail*q->element_size, q->element_size);

    q->tail++;
    if (q->tail >= q->size){
        q->tail = 0;
    }
    q->length--;

    pthread_mutex_unlock(q->mutex);
    return 0;
}


inline bool queue_isEmpty(queue_t *q){
    bool isEmpty = false;
    pthread_mutex_lock(q->mutex);
    isEmpty = (q->length == 0);
    pthread_mutex_unlock(q->mutex);
    return isEmpty;
}