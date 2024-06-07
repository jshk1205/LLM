//실시간 채팅 애플리케이션을 간단하게 만들어줘요.
//현재 이 코드는 서버 영역으로 해줘
//서버가 열렸다는걸 알려줘
//클라이언트가 접속하면 접속했다고 알려줘

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#define MAX_BUFFER 100
#define MAX_CLIENT 10

SOCKET clientSockets[MAX_CLIENT];
int clientCount = 0;

unsigned int __stdcall sendThread(void *arg)
{
    SOCKET serverSocket = *(SOCKET *)arg;
    char buffer[MAX_BUFFER];
    while (1)
    {
        fgets(buffer, MAX_BUFFER, stdin);
        for (int i = 0; i < clientCount; i++)
        {
            send(clientSockets[i], buffer, strlen(buffer), 0);
        }
    }
    return 0;
}

unsigned int __stdcall clientThread(void *arg)
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
        printf("클라이언트: %s\n", buffer); // 클라이언트가 보낸 메시지를 출력합니다.
        for (int i = 0; i < clientCount; i++)
        {
            if (clientSockets[i] != clientSocket) // 자신을 제외한 클라이언트에게만 메시지를 보냅니다.
            {
                send(clientSockets[i], buffer, strlen(buffer), 0);
            }
        }
    }
    closesocket(clientSocket);
    return 0;
}

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(serverSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    printf("서버가 열렸습니다.\n");

    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, sendThread, &serverSocket, 0, NULL);

    while (1)
    {
        SOCKADDR_IN clientAddr;
        int addrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (SOCKADDR *)&clientAddr, &addrLen);
        clientSockets[clientCount++] = clientSocket;
        printf("클라이언트가 접속했습니다.\n");
        HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, clientThread, &clientSocket, 0, NULL);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

