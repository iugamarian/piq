#ifndef __TELEMETRY_H__
#define __TELEMETRY_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "utils.h"
#include "mpu6050.h"


/* DEFINES */
#define IPV4 1
#define IPV6 2


/* STRUCTURES */
struct tcp_client
{
    char *ip;
    int port;
    int protocol;
    int socket;
};


/* FUNCTIONS */
struct tcp_client *tcp_client_new(const char *host, const int port);
void tcp_client_destroy(void *target);
int tcp_client_send(struct tcp_client *c, const char *msg);
char *tcp_client_recv(struct tcp_client *c);
void *telemetry_loop(void *arg);

#endif
