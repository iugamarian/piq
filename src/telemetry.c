#include "telemetry.h"


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

static int tcp_client_setup(struct tcp_client *c)
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
        check(c->socket >= 0, "failed to create TCP socket");

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
        check(retval >= 0, "failed to connect to server");

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

struct tcp_client *tcp_client_new(const char *host, const int port)
{
    int retval;
    struct tcp_client *c;

    c = malloc(sizeof(struct tcp_client));

    c->ip = NULL;
    c->host = malloc(sizeof(char) * (strlen(host) + 1));
    strcpy(c->host, host);
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
    free_mem(c->host, free);
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

int telemetry_loop(void)
{
    struct tcp_client *client;
    char *msg;

    client = tcp_client_new("localhost", 8000);
    silent_check(client != NULL);

    log_info("connected to localhost!");
    while (1) {
        /* send */
        if (tcp_client_send(client, "YES!\n") == -1) {
            break;
        }

        /* receive */
        msg = tcp_client_recv(client);
        if (msg == NULL) {
            break;
        } else {
            log_info("received: %s", msg);
            free(msg);
        }
    }
    log_info("disconnected from localhost!");

    return 0;
error:
    return -1;
}
