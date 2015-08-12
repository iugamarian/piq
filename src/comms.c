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
    struct timeval tv;
    fd_set readfds;

    /* setup */
    ip = "192.168.1.2";
    /* ip = "10.0.0.13"; */
    port = 8000;
    p = arg;

    client = tcp_client_new(ip, port);
    silent_check(client != NULL);
    log_info("connected to %s!", ip);

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    /* events loop */
    while (p->state == PIQ_RUN) {
        FD_ZERO(&readfds);
        FD_SET(client->socket, &readfds);
        select(client->socket + 1, &readfds, NULL, NULL, &tv);

        mpu6050_data(p->imu);

        /* setup */
        memset(buf, '\0', 100);
        sprintf(
            buf,
            "%f %f %f %f %f %f\n",
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
        if (FD_ISSET(client->socket, &readfds)) {
            read_size = read(client->socket, buf, 100);
            check(read_size > 0, "failed to read from server!");
        }

        /* THROTTLE */
        if (strcmp(buf, "]") == 0) {
            log_info("throttle up");
            p->motors->throttle += 0.01;

        } else if (strcmp(buf, "[") == 0) {
            log_info("throttle down");
            p->motors->throttle -= 0.01;

        /* PID PITCH AND ROLL SET POINT */
        } else if (strcmp(buf, "w") == 0) {
            p->motors->pitch_pid->setpoint += 1.0;
            log_info(
                "increase pitch offset to %f",
                p->motors->pitch_pid->setpoint
            );

        } else if (strcmp(buf, "s") == 0) {
            p->motors->pitch_pid->setpoint -= 1.0;
            log_info(
                "decrease pitch offset to %f",
                p->motors->pitch_pid->setpoint
            );

        } else if (strcmp(buf, "a") == 0) {
            p->motors->roll_pid->setpoint -= 1.0;
            log_info(
                "decrease roll offset to %f",
                p->motors->roll_pid->setpoint
            );

        } else if (strcmp(buf, "d") == 0) {
            p->motors->roll_pid->setpoint += 1.0;
            log_info(
                "increase roll offset to %f",
                p->motors->roll_pid->setpoint
            );

        /* IMU PITCH AND ROLL OFFSET */
        } else if (strcmp(buf, "W") == 0) {
            p->imu->pitch_offset += 1.0;
            log_info(
                "increase imu pitch offset to %f",
                p->imu->pitch_offset
            );

        } else if (strcmp(buf, "S") == 0) {
            p->imu->pitch_offset -= 1.0;
            log_info(
                "increase imu pitch offset to %f",
                p->imu->pitch_offset
            );

        } else if (strcmp(buf, "A") == 0) {
            p->imu->roll_offset -= 1.0;
            log_info(
                "increase imu roll offset to %f",
                p->imu->roll_offset
            );

        } else if (strcmp(buf, "D") == 0) {
            p->imu->roll_offset += 1.0;
            log_info(
                "increase imu roll offset to %f",
                p->imu->roll_offset
            );

        /* PID CALIBRATION */
        } else if (strcmp(buf, "p") == 0) {
            p->motors->pitch_pid->k_p += 0.0001;
            log_info(
                "increase pitch pid setting to %f",
                p->motors->pitch_pid->k_p
            );

        } else if (strcmp(buf, "o") == 0) {
            p->motors->pitch_pid->k_p -= 0.0001;
            log_info(
                "decrease pitch pid setting to %f",
                p->motors->pitch_pid->k_p
            );

        } else if (strcmp(buf, "l") == 0) {
            p->motors->roll_pid->k_p += 0.0001;
            log_info(
                "increase pitch pid setting to %f",
                p->motors->roll_pid->k_p
            );

        } else if (strcmp(buf, "k") == 0) {
            p->motors->roll_pid->k_p -= 0.0001;
            log_info(
                "decrease pitch pid setting to %f",
                p->motors->roll_pid->k_p
            );

        /* MISC */
        } else if (strcmp(buf, "r") == 0) {
            log_info("reset pca9685");
            pca9685_reset();

        } else if (strcmp(buf, "c") == 0) {
            log_info("calibrate esc");
            esc_calibrate(p->motors);

        } else if (strcmp(buf, "q") == 0) {
            p->state = PIQ_STOP;
            log_info("halt!");
            break;

        }
        esc_set_throttles(p->motors, p->imu);

    }
    log_info("disconnected from %s!", ip);
    close(client->socket);

    retval = malloc(sizeof(int));
    *retval = 0;
    return retval;
error:
    p->state = 0;
    retval = malloc(sizeof(int));
    *retval = -1;
    return retval;
}
