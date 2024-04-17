#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE_1KB (1024 * 1024 * 1024) // 1KB
#define BUFFER_SIZE_1MB (1024 * 1024) // 1MB

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
    server_address.sin_port = htons(12345);

    // 서버에 연결
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // 데이터 전송
    char* message = (char*)malloc(BUFFER_SIZE_1KB);
    memset(message, 'A', BUFFER_SIZE_1KB); // 1KB 크기의 데이터 생성

    while (1) {
        if (send(client_socket, message, BUFFER_SIZE_1KB, 0) == -1) {
            perror("Send failed");
            close(client_socket);
            free(message);
            exit(EXIT_FAILURE);
        }
        printf("Sent %d bytes of data\n", BUFFER_SIZE_1KB);
        sleep(1); // 1초 대기
    }

    // 메모리 해제 및 소켓 닫기
    free(message);
    close(client_socket);

    return 0;
}
