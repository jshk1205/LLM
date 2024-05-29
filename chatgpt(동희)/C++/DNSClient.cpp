// Ws2_32.lib 종속성 추가해됨

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring> // strcpy_s를 사용하기 위해 필요

#pragma comment(lib, "Ws2_32.lib")

#define SERVER_PORT "1053" // 변경된 포트 번호
#define SERVER_ADDRESS "127.0.0.1"
#define DNS_QUERY "example.com"

// Initialize Winsock
bool InitializeWinsock();
// Create a socket and send DNS query
void SendDNSQuery(const char* serverAddress, const char* port, const char* query);

int main() {
    if (!InitializeWinsock()) {
        std::cerr << "Winsock initialization failed" << std::endl;
        return 1;
    }

    SendDNSQuery(SERVER_ADDRESS, SERVER_PORT, DNS_QUERY);

    WSACleanup();

    // Wait for user input before closing
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}

// Initialize Winsock
bool InitializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return false;
    }
    return true;
}

// Create a socket and send DNS query
void SendDNSQuery(const char* serverAddress, const char* port, const char* query) {
    addrinfo hints = {}, * res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    int result = getaddrinfo(serverAddress, port, &hints, &res);
    if (result != 0) {
        std::cerr << "getaddrinfo failed with error: " << result << std::endl;
        return;
    }

    SOCKET querySocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (querySocket == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(res);
        return;
    }

    char buffer[512];
    strcpy_s(buffer, sizeof(buffer), query);
    int queryLength = static_cast<int>(strlen(buffer) + 1); // size_t to int 변환

    result = sendto(querySocket, buffer, queryLength, 0, res->ai_addr, static_cast<int>(res->ai_addrlen)); // size_t to int 변환
    if (result == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(res);
        closesocket(querySocket);
        return;
    }

    sockaddr_in serverAddr;
    int serverAddrLen = sizeof(serverAddr);
    int bytesReceived = recvfrom(querySocket, buffer, sizeof(buffer), 0, (sockaddr*)&serverAddr, &serverAddrLen);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "recvfrom failed with error: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "Received response: " << buffer << std::endl;
    }

    freeaddrinfo(res);
    closesocket(querySocket);
}
