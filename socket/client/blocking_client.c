#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_NAME "blocking_server" // Docker Compose에서 정의한 서비스 이름

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

    struct hostent *server = gethostbyname(SERVER_NAME);
    if (server == NULL) {
        fprintf(stderr, "Error: Unable to resolve hostname\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0],
           server->h_length);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    send(sock, hello, strlen(hello), 0);
    printf("Blocking client sent message\n");
    fflush(stdout);

    ssize_t valread = read(sock, buffer, 1024);
    if (valread > 0) {
        printf("Blocking client received: %s\n", buffer);
        fflush(stdout);
    }
    else {
        perror("read");
    }

    close(sock);
    return 0;
}
