#include "comms.h"


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

void *comms_loop(void *arg)
{
    char *ip;
    int port;
    int *retval;
    size_t read_size;
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
        sprintf(
            buf,
            "%f %f %d %d %d %d",
            p->imu->pitch,
            p->imu->roll,
            p->motors->motor_1,
            p->motors->motor_2,
            p->motors->motor_3,
            p->motors->motor_4
        );

        /* send */
        if (tcp_client_send(client, buf) == -1) {
            break;
        }

        /* receive */
        memset(buf, '\0', 100);
        read_size = read(client->socket, buf, 100);
        check(read_size > 0, "failed to read from server!");
        if (strcmp(buf, ".") == 0) {
            // do nothing

        } else if (strcmp(buf, "w") == 0) {
            log_info("throttle up");
            p->motors->motor_1 += 100;
            p->motors->motor_2 += 100;
            p->motors->motor_3 += 100;
            p->motors->motor_4 += 100;
            esc_set_throttles(p->motors);

        } else if (strcmp(buf, "s") == 0) {
            log_info("throttle down");
            p->motors->motor_1 -= 100;
            p->motors->motor_2 -= 100;
            p->motors->motor_3 -= 100;
            p->motors->motor_4 -= 100;
            esc_set_throttles(p->motors);

        } else if (strcmp(buf, "r") == 0) {
            log_info("reset pca9685");
            pca9685_reset();

        } else if (strcmp(buf, "c") == 0) {
            log_info("calibrate esc");
            esc_calibrate(p->motors);

        } else if (strcmp(buf, "q") == 0) {
            p->imu->state = 0;
            log_info("quit telemetry loop!");
            break;

        }
    }
    log_info("disconnected from %s!", ip);
    close(client->socket);

    retval = malloc(sizeof(int));
    *retval = 0;
    return retval;
error:
    p->imu->state = 0;
    retval = malloc(sizeof(int));
    *retval = -1;
    return retval;
}
