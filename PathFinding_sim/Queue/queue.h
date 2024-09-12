#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdint.h>
#include <pthread.h>
#include "utility.h"


typedef struct {
    uint8_t *data;
    uint head;
    uint tail;
    uint size;
    uint length;
    uint element_size;
    pthread_mutex_t *mutex;
} queue_t;



/*! \brief Initialise a queue
 *  \ingroup queue
 *
 * \param q Pointer to a queue_t structure, used as a handle
 * \param element_size Size of each value in the queue
 * \param size Maximum number of entries in the queue
 */
void queue_init(queue_t *q, uint element_size, uint size);

/*! \brief Destroy queue
 *  \ingroup queue
 *
 * \param q Pointer to a queue_t
 * Does not deallocate queue_t structure itself!
*/
void queue_free(queue_t *q);

int queue_push(queue_t *q, void *data);
int queue_pop(queue_t *q, void *data);

extern bool queue_isEmpty(queue_t *q);

#endif
