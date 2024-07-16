#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_IP                                                              \
    "172.18.0.3" // Use the service name defined in docker-compose.yml

int
main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char *hello = "Hello from client";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    // Set the socket to non-blocking mode
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    while (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
           0) {
        if (errno == EINPROGRESS) {
            printf("Non-blocking client: connection in progress\n");
            fflush(stdout);
            usleep(1000000); // Sleep for 1 seconds to prevent tight loop
            continue;
        }
        else {
            perror("Connection Failed");
            return -1;
        }
    }

    send(sock, hello, strlen(hello), 0);
    printf("Non-blocking client sent message\n");
    fflush(stdout);

    ssize_t valread;
    while ((valread = read(sock, buffer, 1024)) <= 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            printf("Non-blocking client: no data available to read yet\n");
            fflush(stdout);
            usleep(1000000); // Sleep for 1 seconds to prevent tight loop
            continue;
        }
        else {
            perror("read");
            close(sock);
            return -1;
        }
    }
    printf("Non-blocking client received: %s\n", buffer);
    fflush(stdout);

    close(sock);
    return 0;
}
