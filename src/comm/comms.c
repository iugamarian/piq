#include "piq/comm/comm.h"


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
    check(retval == 0, "failed to connect to %s", c->ip);
    retval = fcntl(c->socket, F_SETFL, O_NONBLOCK);
    check(retval != -1, "failed to set socket as non-blocking!");

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
