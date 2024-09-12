#include "udp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "queue.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utility.h"

static int sockfd;
static char read_buffer[UDP_PACKET_SIZE];
static struct sockaddr_in server_address, client_address;
static queue_t send_queue;
static queue_t recv_queue;

static bool run;

int udp_init(){
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_address, 0, sizeof(server_address));
    memset(&client_address, 0, sizeof(client_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(UDP_SERVER_IP);
    server_address.sin_port = htons(UDP_SERVER_PORT);

    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(UDP_CLIENT_IP);
    client_address.sin_port = htons(UDP_CLIENT_PORT);


    int status = bind(sockfd, (const struct sockaddr *)&server_address, sizeof(server_address));
    if (status < 0){
        perror("Bind failed");
        return status;
    }

    return 0;
}



void* udp_run(void *vargp){
    int status = udp_init();
    if (status != 0){
        printf(T_RED "UDP failed init\n" T_RESET);
        return NULL;
    }
    queue_init(&send_queue, UDP_PACKET_SIZE, 10);
    queue_init(&recv_queue, UDP_PACKET_SIZE, 10);
    run = true;
    char buffer[UDP_PACKET_SIZE];
    socklen_t len;
    int n;
    

    while (run){
        n = recvfrom(sockfd, buffer, UDP_PACKET_SIZE, MSG_WAITALL, (struct sockaddr *)&server_address, &len);
        queue_push(&recv_queue, buffer);
    }
}

void udp_send(const char *format, ...){
    char msg[UDP_PACKET_SIZE] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(msg, UDP_PACKET_SIZE-1, format, args);
    va_end(args);

    sendto(sockfd, msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *)&client_address, sizeof(client_address));
}

int udp_read(char *buffer){
    int status = queue_pop(&recv_queue, buffer);
    if (status != 0){
        buffer = NULL;
    }
    return strlen(buffer);
}

int udp_tryRead(char *buffer){
    if (!queue_isEmpty(&recv_queue)){
        return udp_read(buffer);
    }
    return 0;
}

void udp_server_stop(){
    char msg[UDP_PACKET_SIZE] = {0};
    
    printf("Close UDP thread\n");
    close(sockfd);
    run = false;
}

