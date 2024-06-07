//실시간 채팅 애플리케이션을 간단하게 만들어줘요.
//현재 이 코드는 클라이언트 영역으로 해줘
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#define MAX_BUFFER 100

unsigned int __stdcall receiveThread(void *arg)
{
    SOCKET clientSocket = *(SOCKET *)arg;
    char buffer[MAX_BUFFER];
    while (1)
    {
        int length = recv(clientSocket, buffer, MAX_BUFFER, 0);
        if (length <= 0)
        {
            break;
        }
        buffer[length] = 0;
        printf("서버: %s\n", buffer);
    }
    closesocket(clientSocket);
    return 0;
}

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(clientSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));

    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, receiveThread, &clientSocket, 0, NULL);

    char buffer[MAX_BUFFER];
    while (1)
    {
        fgets(buffer, MAX_BUFFER, stdin);
        printf("나: %s", buffer); // 사용자가 입력한 메시지를 출력합니다.
        send(clientSocket, buffer, strlen(buffer), 0);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}