#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    // 소켓 생성
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    char buffer[BUFFER_SIZE] = {0};
    while(1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
            perror("Send failed");
            close(client_socket);
            exit(EXIT_FAILURE);
            break;
        }
    }

    close(client_socket);
    return 0;
}
