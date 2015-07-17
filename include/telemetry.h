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
    char *host;
    int port;
    int protocol;
    int socket;
};

struct telemetry
{
    struct tcp_client *client;
    struct mpu6050_data *imu;
    struct esc *throttle;
};


/* FUNCTIONS */
struct tcp_client *tcp_client_new(void);
void tcp_client_destroy(void *target);
int tcp_client_setup(struct tcp_client *c);
int tcp_client_send(struct tcp_client *c, const char *msg);
char *tcp_client_recv(struct tcp_client *c);

struct telemetry *telemetry_new(struct mpu6050_data *imu, struct esc *throttle);
void telemetry_destroy(void *target);
int telemetry_setup(struct telemetry *t, const char *host, int port);
int telemetry_send(struct telemetry *t);
char *telemetry_recv(struct telemetry *t);

#endif
