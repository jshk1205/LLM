#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define _CRT_SECURE_NO_WARNINGS
#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];
    FILE* file;
    long fileSize;
    size_t bytesReceived, totalBytesReceived = 0;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Failed to create socket.\n");
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen)) == INVALID_SOCKET) {
        printf("Accept failed.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Client connected.\n");

    if ((bytesReceived = recv(clientSocket, (char*)&fileSize, sizeof(fileSize), 0)) == SOCKET_ERROR) {
        printf("Failed to receive file size.\n");
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    fileSize = ntohl(fileSize);
    if (fileSize < 0 || fileSize > 1024 * 1024 * 1024) {
        printf("Invalid file size received: %ld\n", fileSize);
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Receiving file of size: %ld bytes\n", fileSize);

    errno_t err = fopen_s(&file, "received_file.txt", "wb");
    if (err != 0 || file == NULL) {
        printf("Failed to create file.\n");
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    while (totalBytesReceived < fileSize) {
        bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            printf("Failed to receive file data.\n");
            fclose(file);
            closesocket(clientSocket);
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }
        totalBytesReceived += bytesReceived;
        fwrite(buffer, 1, bytesReceived, file);
    }

    printf("File received successfully.\n");

    fclose(file);
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
