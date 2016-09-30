#ifndef __TELEMETRY_H__
#define __TELEMETRY_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "util.h"


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

#endif
