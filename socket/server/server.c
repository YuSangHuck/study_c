#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#define PORT 12345
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int i, client_sockets[MAX_CLIENTS];
    // Initialize client sockets to 0
    for (i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // Creating socket file descriptor
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    // 바인딩
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // 수신 대기
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    int new_socket, activity, valread;
    int addrlen = sizeof(server_address);
    char buffer[BUFFER_SIZE] = {0};
    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);
        int max_sd = server_socket;

        for (i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_sockets[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno!=EINTR)) {
            perror("select error");
        }

        if (FD_ISSET(server_socket, &readfds)) {
            if ((new_socket = accept(server_socket, (struct sockaddr *)&server_address, (socklen_t*)&addrlen))<0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            // Send welcome message to the client
            printf("New client connected. IP: %s, Port: %d\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

            // Add new socket to array of sockets
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        for (i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_sockets[i];

            if (FD_ISSET(sd, &readfds)) {
                if ((valread = read(sd, buffer, BUFFER_SIZE)) == 0) {
                    // Client disconnected
                    getpeername(sd, (struct sockaddr*)&server_address, (socklen_t*)&addrlen);
                    printf("Host disconnected, IP %s, Port %d \n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

                    // Close the socket and mark as 0 in list
                    close(sd);
                    client_sockets[i] = 0;
                } else {
                    // Print received message
                    buffer[valread] = '\0';
                    printf("Client: %s", buffer);
                }
            }
        }
    }
    return 0;
}
