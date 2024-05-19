#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <WS2tcpip.h> // For inet_pton function

#define PORT 53
#define SERVER_IP "127.0.0.1" // 서버의 IP 주소

#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    int recv_len;
    char buffer[BUFFER_SIZE];

    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");

    server.sin_family = AF_INET;
    // inet_addr 함수를 inet_pton 함수로 대체
    if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) != 1) {
        printf("Invalid address: %s", SERVER_IP);
        return 1;
    }
    server.sin_port = htons(PORT);

    // DNS 쿼리 생성
    char query[] = "\x00\x01\x01\x00\x00\x01\x00\x00\x00\x00\x00\x00\x03www\x06google\x03com\x00\x00\x01\x00\x01";
    int query_len = strlen(query);

    // 서버에 DNS 쿼리 전송
    if (sendto(s, query, query_len, 0, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("sendto() failed with error code : %d", WSAGetLastError());
        return 1;
    }

    printf("DNS query sent.\n");

    // 서버로부터 응답 받기
    memset(buffer, 0, BUFFER_SIZE);
    if ((recv_len = recv(s, buffer, BUFFER_SIZE, 0)) == SOCKET_ERROR) {
        printf("recv() failed with error code : %d", WSAGetLastError());
        return 1;
    }

    printf("DNS response received: %s\n", buffer);

    closesocket(s);
    WSACleanup();

    return 0;
}
