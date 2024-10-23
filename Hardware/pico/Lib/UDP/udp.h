#ifndef __UDP__H
#define __UDP__H

#include <pico/multicore.h>
#include "lwip/err.h"

#define UDP_MSG_LEN_MAX 128
#define UDP_SEND_QUEUE_SIZE 100
#define UDP_RECV_QUEUE_SIZE 20

void UDP_initQueue();
void UDP_main();

// Put message on queue
// void UDP_send(const char *msg);
void UDP_send(const char *format, ...);

// Get message from queue
bool UDP_read(const char *msg);

#endif