#ifndef __UDP_H__
#define __UDP_H__

#include <pthread.h>

#define UDP_SERVER_PORT 4444
#define UDP_SERVER_IP "10.42.0.1"

#define UDP_CLIENT_PORT 4445
// #define UDP_CLIENT_IP "10.42.0.210"
#define UDP_CLIENT_IP "10.42.0.22"
// #define UDP_CLIENT_IP "10.42.0.1"
#define UDP_PACKET_SIZE 128


void* udp_run(void *vargp);

void udp_server_stop();

int udp_read(char *buffer);
int udp_tryRead(char *buffer);
void udp_send(const char *format, ...);

#endif