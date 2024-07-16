#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
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
    const char *hello = "Hello from non-blocking server";

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

    // Set the socket to non-blocking mode
    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    while (1) {
        printf("Non-blocking server waiting for connection...\n");
        fflush(stdout);

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                // No pending connections; non-blocking mode means accept()
                // returns immediately
                usleep(
                    1000000); // Sleep for 100 milliseconds to prevent tight loop
                continue;
            }
            else {
                perror("accept");
                close(server_fd);
                exit(EXIT_FAILURE);
            }
        }

        read(new_socket, buffer, 1024);
        printf("Non-blocking server received: %s\n", buffer);
        fflush(stdout);
        send(new_socket, hello, strlen(hello), 0);
        printf("Non-blocking server sent hello message\n");
        fflush(stdout);

        close(new_socket);
    }

    close(server_fd);
    return 0;
}
