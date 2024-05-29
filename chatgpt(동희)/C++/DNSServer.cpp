// Ws2_32.lib 종속성 추가해됨

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "1053" // DNS uses port 53

// Initialize Winsock
bool InitializeWinsock();
// Create and bind a socket
SOCKET CreateBindSocket(const char* port);
// Handle incoming DNS requests
void HandleDNSRequests(SOCKET listenSocket);
// Parse DNS query
void ParseDNSQuery(char* buffer, int length);
// Build DNS response
void BuildDNSResponse(char* buffer, int& length);
// Send DNS response
void SendDNSResponse(SOCKET clientSocket, sockaddr_in* clientAddr, int clientAddrLen, const char* response, int length);

int main() {
    if (!InitializeWinsock()) {
        std::cerr << "Winsock initialization failed" << std::endl;
        return 1;
    }

    // Try to create and bind the socket, if port 53 fails, use another port
    SOCKET listenSocket = CreateBindSocket(DEFAULT_PORT);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation or binding failed on port " << DEFAULT_PORT << ", trying another port..." << std::endl;
        listenSocket = CreateBindSocket("1053"); // Alternate port
        if (listenSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation or binding failed again" << std::endl;
            WSACleanup();
            return 1;
        }
    }

    HandleDNSRequests(listenSocket);

    closesocket(listenSocket);
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

// Create and bind a socket
SOCKET CreateBindSocket(const char* port) {
    addrinfo hints = {}, * res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_PASSIVE;

    int result = getaddrinfo(nullptr, port, &hints, &res);
    if (result != 0) {
        std::cerr << "getaddrinfo failed with error: " << result << std::endl;
        return INVALID_SOCKET;
    }

    SOCKET listenSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(res);
        return INVALID_SOCKET;
    }

    result = bind(listenSocket, res->ai_addr, (int)res->ai_addrlen);
    if (result == SOCKET_ERROR) {
        std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(res);
        closesocket(listenSocket);
        return INVALID_SOCKET;
    }

    freeaddrinfo(res);
    return listenSocket;
}

// Handle incoming DNS requests
void HandleDNSRequests(SOCKET listenSocket) {
    char buffer[512];
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    int bytesReceived;

    while (true) {
        bytesReceived = recvfrom(listenSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientAddrLen);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "recvfrom failed with error: " << WSAGetLastError() << std::endl;
            continue;
        }

        ParseDNSQuery(buffer, bytesReceived);

        int responseLength = bytesReceived;
        BuildDNSResponse(buffer, responseLength);

        SendDNSResponse(listenSocket, &clientAddr, clientAddrLen, buffer, responseLength);
    }
}

// Parse DNS query
void ParseDNSQuery(char* buffer, int length) {
    // For simplicity, we'll skip detailed parsing.
    // Normally, you would parse the DNS header and question sections.
    std::cout << "Received DNS query" << std::endl;
}

// Build DNS response
void BuildDNSResponse(char* buffer, int& length) {
    // For simplicity, we'll just echo the query back as the response.
    // Normally, you would construct a proper DNS response.
    std::cout << "Building DNS response" << std::endl;
}

// Send DNS response
void SendDNSResponse(SOCKET clientSocket, sockaddr_in* clientAddr, int clientAddrLen, const char* response, int length) {
    int bytesSent = sendto(clientSocket, response, length, 0, (sockaddr*)clientAddr, clientAddrLen);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
    }
}
