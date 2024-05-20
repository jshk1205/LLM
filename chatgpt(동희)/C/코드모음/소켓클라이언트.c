#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "comdlg32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define BUFFER_SIZE 1024
#define MAX_FILE_PATH 260

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        printf("Failed to create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) != 1) {
        printf("Invalid address: %s\n", SERVER_IP);
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    serverAddr.sin_port = htons(PORT);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Failed to connect to server: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    OPENFILENAME ofn;
    TCHAR szFile[MAX_FILE_PATH] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files (*.txt)\0*.TXT\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        printf("Selected file: %s\n", szFile);

        FILE* file = fopen(szFile, "rb");
        if (file == NULL) {
            printf("Failed to open file: %s\n", szFile);
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        long fileSizeNetworkOrder = htonl(fileSize);
        printf("Sending file size: %ld bytes\n", fileSize);

        if (send(clientSocket, (char*)&fileSizeNetworkOrder, sizeof(fileSizeNetworkOrder), 0) == SOCKET_ERROR) {
            printf("Failed to send file size: %d\n", WSAGetLastError());
            fclose(file);
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        char buffer[BUFFER_SIZE];
        int bytesRead;
        while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
            if (send(clientSocket, buffer, bytesRead, 0) == SOCKET_ERROR) {
                printf("Failed to send data: %d\n", WSAGetLastError());
                fclose(file);
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
        }

        fclose(file);
        printf("File sent successfully.\n");
    }
    else {
        printf("No file selected.\n");
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
