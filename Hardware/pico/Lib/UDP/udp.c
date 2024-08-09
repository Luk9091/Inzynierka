#include "udp.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <pico/util/queue.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "PASS.h"

#define UDP_PORT 4444
#define UDP_INTERVAL_MS 100

static struct udp_pcb* pcb;
static ip_addr_t addr;
static ip_addr_t server;

static queue_t send_queue;
static queue_t recv_queue;


int get_IP(char *out){
    extern cyw43_t cyw43_state;
    uint ip_addr = cyw43_state.netif[CYW43_ITF_STA].ip_addr.addr;
    sprintf(out, "%lu.%lu.%lu.%lu", ip_addr & 0xFF, (ip_addr >> 8) & 0xFF, (ip_addr >> 16) & 0xFF, ip_addr >> 24);
    // printf("IP Address: %s\n", out);
    return strlen(out);
}

err_t UDP_write(const char *msg){
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, UDP_MSG_LEN_MAX, PBUF_RAM);
    char *req = (char *)p->payload;
    memset(req, 0, UDP_MSG_LEN_MAX);
    snprintf(req, UDP_MSG_LEN_MAX-1, "%s", msg);
    err_t er = udp_sendto(pcb, p, &addr, UDP_PORT);
    pbuf_free(p);

    return er;
}


static void UDP_recvCallback(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t port) {
    LWIP_UNUSED_ARG(arg);

    if (p == NULL){
        return;
    }
    char msg[UDP_MSG_LEN_MAX] = {0};
    memcpy(msg, (char *)p->payload, p->len);
    queue_try_add(&recv_queue, msg);
    // queue_try_add(&send_queue, msg);

    pbuf_free(p);
}

int UDP_init(){
    cyw43_arch_init_with_country(CYW43_COUNTRY_POLAND);
    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } 
    printf("Connected.\n");
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1); 


    pcb = udp_new();
    
    char ip[16] = {0};
    get_IP(ip);
    ipaddr_aton(ip, &server);
    printf("IP Address: %s\n", ip);

    ipaddr_aton(WIFI_CLIENT_IP, &addr);

    udp_bind(pcb, &server, UDP_PORT);
    udp_recv(pcb, (udp_recv_fn)UDP_recvCallback, pcb);

    return 0;
}

void UDP_main() {
    multicore_lockout_victim_init();
    queue_init(&send_queue, UDP_MSG_LEN_MAX, UDP_SEND_QUEUE_SIZE);
    queue_init(&recv_queue, UDP_MSG_LEN_MAX, UDP_RECV_QUEUE_SIZE);

    if (UDP_init() != 0) {
        cyw43_arch_deinit();
        printf("UDP init failed\n");
        return;
    }
    UDP_write("Run server\n");


    while (true) {
        while (!queue_is_empty(&send_queue)){
            char msg[UDP_MSG_LEN_MAX] = {0};
            queue_remove_blocking(&send_queue, msg);
            UDP_write(msg);
        }
        // UDP_write("Hello World");
        // Note in practice for this simple UDP transmitter,
        // the end result for both background and poll is the same

#if PICO_CYW43_ARCH_POLL
        // if you are using pico_cyw43_arch_poll, then you must poll periodically from your
        // main loop (not from a timer) to check for Wi-Fi driver or lwIP work that needs to be done.
        cyw43_arch_poll();
        sleep_ms(UDP_INTERVAL_MS);
#else
        // if you are not using pico_cyw43_arch_poll, then WiFI driver and lwIP work
        // is done via interrupt in the background. This sleep is just an example of some (blocking)
        // work you might be doing.
        sleep_ms(UDP_INTERVAL_MS);
#endif
    }
}




void UDP_send(const char *format, ...){
    char msg[UDP_MSG_LEN_MAX] = {0};
    va_list args;
    
    va_start(args, format);
    vsnprintf(msg, UDP_MSG_LEN_MAX-1, format, args);
    va_end(args);

    queue_add_blocking(&send_queue, msg);
}


bool UDP_read(const char *msg){
    return queue_try_remove(&recv_queue, (void *)msg);
}


