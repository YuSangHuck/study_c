#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#define BUFFER_SIZE (1024 * 1024) // 1MB

int main() {
    // 소켓 생성
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(12345);

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

    // 클라이언트 연결 수락
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        perror("Accept failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // 소켓 버퍼의 최대 크기 확인
    int max_buffer_size;
    if (ioctl(client_socket, FIONREAD, &max_buffer_size) == -1) {
        perror("Failed to get max buffer size");
        close(client_socket);
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Max buffer size: %d byes\n", max_buffer_size);

    // 클라이언트로부터 데이터 수신
    char buffer[BUFFER_SIZE];
    while (1) {
        // int recv_size = recv(client_socket, buffer, BUFFER_SIZE, 0); // MSG_DONTWAIT 플래그 제거
        // if (recv_size == -1) {
        //     perror("Receive failed");
        //     close(client_socket);
        //     close(server_socket);
        //     exit(EXIT_FAILURE);
        // } else if (recv_size == 0) {
        //     printf("Client disconnected\n");
        //     break;
        // } else {
        //     printf("Received %d bytes of data\n", recv_size);
        //     // 여기서 데이터를 처리하지 않고 소켓 버퍼를 계속 채우도록 함
        // }
        sleep(1);
    }


    // 소켓 닫기
    close(client_socket);
    close(server_socket);

    return 0;
}
