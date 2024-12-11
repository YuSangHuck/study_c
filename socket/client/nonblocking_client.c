#include "../common/log.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT             8080
#define SERVER_NAME      "blocking_server"
#define CONNECT_INTERVAL 1000000 // 1s in microseconds
#define BUFFER_SIZE      1024
#define SEND_INTERVAL    1000000 // 100ms in microseconds

int
main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char *hello = "Hello from client";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG("socket creation failed (error:%s)", strerror(errno));
        return -1;
    }


    struct hostent *server = gethostbyname(SERVER_NAME);
    if (server == NULL) {
        LOG("Unable to resolve hostname");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0],
           server->h_length);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        LOG("connect failed (error:%s)", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    while (1) {
        // 보내기 시도
        LOG("try to send");
        int n = send(sock, buffer, BUFFER_SIZE, 0);
        if (n < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                LOG("send buffer full, EWOULDBLOCK");
            }
            else {
                LOG("send failed (error:%s)", strerror(errno));
                close(sock);
                exit(EXIT_FAILURE);
            }
        }
        else {
            LOG("sended");
        }

        // SEND_INTERVAL 동안 대기
        usleep(SEND_INTERVAL);
    }

    close(sock);
    return 0;
}
