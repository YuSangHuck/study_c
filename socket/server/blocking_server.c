#include "../common/log.h"
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT          8080
#define BUFFER_SIZE   1024
#define RECV_INTERVAL 1000000 // 1 second in microseconds

int
main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *hello = "Hello from blocking server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        LOG("socket failed (error:%s)", strerror(errno));
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        LOG("bind failed (error:%s)", strerror(errno));
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        LOG("listen (error:%s)", strerror(errno));
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    LOG("waiting for connection...");

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0) {
        LOG("accept (error:%s)", strerror(errno));
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    LOG("accepted connection");

    while (1) {
        // 데이터 수신
        int n = recv(server_fd, buffer, BUFFER_SIZE, 0);
        if (n < 0) {
            LOG("recv (error:%s)", strerror(errno));
            close(server_fd);
            close(new_socket);
            exit(EXIT_FAILURE);
        }

        LOG("received %d bytes", n);

        // RECV_INTERVAL 동안 대기
        usleep(RECV_INTERVAL);
    }

    close(server_fd);
    close(new_socket);
    return 0;
}
