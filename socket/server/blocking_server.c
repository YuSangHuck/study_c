#include "../common/log.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080

int
main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *hello = "Hello from blocking server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        LOG("Blocking server waiting for connection...\n");
        fflush(stdout);

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        LOG("Blocking server accepted connection\n");
        fflush(stdout);

        ssize_t valread = read(new_socket, buffer, 1024);
        if (valread > 0) {
            LOG("Blocking server received: %s\n", buffer);
            fflush(stdout);
            send(new_socket, hello, strlen(hello), 0);
            LOG("Blocking server sent hello message\n");
            fflush(stdout);
        }
        else {
            perror("read");
        }

        close(new_socket);
    }

    close(server_fd);
    return 0;
}
