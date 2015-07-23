#include "telemetry.h"


static int tcp_client_setup(struct tcp_client *c)
{
    int retval;
    struct sockaddr_in sock_ipv4;

    /* pre-check */
    check(c != NULL, "TCP client is NULL!");
    check(c->ip != NULL, "tcp_client->ip is NULL!");
    check(c->socket == -1, "tcp_client->socket is already set!");

    /* create socket */
    c->socket = socket(AF_INET, SOCK_STREAM, 0);
    check(c->socket >= 0, "failed to create TCP socket!");

    /* socket address */
    memset(&sock_ipv4, '0', sizeof(sock_ipv4));

    sock_ipv4.sin_family = AF_INET;
    sock_ipv4.sin_port = htons((uint16_t) c->port);
    retval = inet_pton(AF_INET, c->ip, &sock_ipv4.sin_addr);
    check(retval == 1, "invalid ip addr!");

    retval = connect(
        c->socket,
        (struct sockaddr *) &sock_ipv4,
        sizeof(sock_ipv4)
    );
    check(retval == 0, "failed to connect to server! %s", strerror(errno));

    return 0;

error:
    if (c->socket >= 0) {
        return -1;
    } else {
        return -2;
    }
}

struct tcp_client *tcp_client_new(const char *ip, const int port)
{
    int retval;
    struct tcp_client *c;

    c = malloc(sizeof(struct tcp_client));

    c->ip = malloc(sizeof(char) * (strlen(ip) + 1));
    strcpy(c->ip, ip);
    c->port = port;
    c->protocol = IPV4;
    c->socket = -1;

    retval = tcp_client_setup(c);
    if (retval != 0) {
        tcp_client_destroy(c);
        return NULL;
    }

    return c;
}

void tcp_client_destroy(void *target)
{
    struct tcp_client *c = target;

    silent_check(c);
    free_mem(c->ip, free);
    if (c->socket >= 0) {
        close(c->socket);
    }
    free(c);

error:
    return;
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


void *telemetry_loop(void *arg)
{
    char *ip;
    int port;
    char *msg;
    int *retval;
    char buf[100];
    struct tcp_client *client;
    struct piq *p;

    /* setup */
    ip = "10.0.0.13";
    port = 8000;
    p = arg;

    client = tcp_client_new(ip, port);
    silent_check(client != NULL);
    log_info("connected to %s!", ip);

    while (p->imu->state) {
        /* setup */
        memset(buf, '\0', 100);
        sprintf(buf, "%f %f", p->imu->pitch, p->imu->roll);

        /* send */
        if (tcp_client_send(client, buf) == -1) {
            printf("sent %s\n", buf);
            break;
        }

        /* receive */
        msg = tcp_client_recv(client);
        if (strcmp(msg, ".") == 0) {
            // do nothing

        } else if (strcmp(msg, "w") == 0) {
            log_info("throttle up");

        } else if (strcmp(msg, "d") == 0) {
            log_info("throttle down");

        } else if (strcmp(msg, "q") == 0) {
            p->imu->state = 0;
            log_info("quit telemetry loop!");
            break;

        }

        if (msg) {
            free(msg);
            msg = NULL;
        }

        /* usleep(100); */
    }
    log_info("disconnected from %s!", ip);
    close(client->socket);

    retval = malloc(sizeof(int));
    *retval = 0;
    return retval;
error:
    retval = malloc(sizeof(int));
    *retval = -1;
    return retval;
}
