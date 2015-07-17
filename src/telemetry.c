#include "telemetry.h"


struct tcp_client *tcp_client_new(void)
{
    struct tcp_client *c;

    c = malloc(sizeof(struct tcp_client));

    c->ip = NULL;
    c->host = NULL;
    c->port = 80;
    c->protocol = IPV4;
    c->socket = -1;

    return c;
}

void tcp_client_destroy(void *target)
{
    struct tcp_client *c = target;

    silent_check(c);
    free_mem(c->ip, free);
    free_mem(c->host, free);
    if (c->socket >= 0) {
        close(c->socket);
    }
    free(c);

error:
    return;
}

static char *hostname_to_ip(char *hostname)
{
    int retval;
    char *ip;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;
    struct sockaddr_in *h;

    /* hints */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  /* use AF_INET6 to force IPv6 */
    hints.ai_socktype = SOCK_STREAM;

    /* get addrinfo */
    retval = getaddrinfo(hostname , "http" , &hints , &servinfo);
    check(retval == 0, "getaddrinfo: %s\n", gai_strerror(retval));

    /* loop through results and connect to the first we can */
    ip = malloc(sizeof(char) * INET_ADDRSTRLEN);
    for (p = servinfo; p != NULL; p = p->ai_next) {
        h = (struct sockaddr_in *) p->ai_addr;
        inet_ntop(AF_INET, &(h->sin_addr), ip, INET_ADDRSTRLEN);
    }
    freeaddrinfo(servinfo);

    return ip;
error:
    return NULL;
}

int tcp_client_setup(struct tcp_client *c)
{
    int retval;
    struct sockaddr_in sock_ipv4;

    /* pre-check */
    check(c != NULL, "TCP client is NULL!");
    check(c->host != NULL, "tcp_client->host is NULL!");
    check(c->socket == -1, "tcp_client->socket is set!");

    if (c->protocol == IPV4) {
        /* create socket */
        c->socket = socket(AF_INET, SOCK_STREAM, 0);
        check(c->socket >= 0, "Failed to create TCP socket");

        /* socket address */
        memset(&sock_ipv4, '0', sizeof(sock_ipv4));

        sock_ipv4.sin_family = AF_INET;
        sock_ipv4.sin_port = htons((uint16_t) c->port);
        c->ip = hostname_to_ip(c->host);
        retval = inet_pton(AF_INET, c->ip, &sock_ipv4.sin_addr);

        retval = connect(
            c->socket,
            (struct sockaddr *) &sock_ipv4,
            sizeof(sock_ipv4)
        );
        check(retval >= 0, "Failed to connect to server");

    } else if (c->protocol == IPV6) {
        log_err("IPV6 is not supported!");
        return -1;

    } else {
        return -1;

    }

    return 0;
error:
    return -1;
}

int tcp_client_send(struct tcp_client *c, const char *msg)
{
    return write(c->socket, msg, strlen(msg));
}

char *tcp_client_recv(struct tcp_client *c)
{
    size_t read_size;
    char msg[100];
    char *retval;

    read_size = read(c->socket, msg, 100);
    silent_check(read_size > 0);
    retval = malloc(sizeof(char) * (strlen(msg) + 1));
    strcpy(retval, msg);

    return retval;
error:
    return NULL;
}

struct telemetry *telemetry_new(struct mpu6050_data *imu, struct esc *throttle)
{
    struct telemetry *t;

    t = malloc(sizeof(struct telemetry));
    t->client = tcp_client_new();
    t->imu = imu;
    t->throttle = throttle;

    return t;
}

void telemetry_destroy(void *target)
{
    struct telemetry *t;
    t = target;
    tcp_client_destroy(t->client);
    free(t);
}

int telemetry_setup(struct telemetry *t, const char *host, int port)
{
    /* set host */
    t->client->host = malloc(sizeof(char) + (strlen(host) + 1));
    strcpy(t->client->host, host);

    /* set port */
    t->client->port = port;

    /* setup client */
    return tcp_client_setup(t->client);;
}

int telemetry_send(struct telemetry *t)
{
    char msg[100];
    sprintf(msg, "pitch: %f\n", t->imu->pitch);
    return tcp_client_send(t->client, msg);
}

char *telemetry_recv(struct telemetry *t)
{
    return tcp_client_recv(t->client);
}
