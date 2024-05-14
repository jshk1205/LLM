#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

void send_response(SOCKET client_socket, const std::string& response) {
    send(client_socket, response.c_str(), response.size(), 0);
}

void handle_request(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    char method[10], path[255], protocol[10];

    recv(client_socket, buffer, BUFFER_SIZE, 0);
    sscanf(buffer, "%s %s %s", method, path, protocol);

    // Convert path to lowercase
    for (int i = 0; i < strlen(path); i++) {
        path[i] = tolower(path[i]);
    }

    if (strcmp(method, "GET") == 0) {
        if (strcmp(path, "/") == 0) {
            // Default homepage
            std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Welcome to the Home Page!</h1></body></html>\n";
            send_response(client_socket, response);
        }
        else if (strcmp(path, "/about") == 0) {
            // About page
            std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>About Us</h1><p>This is the about page.</p></body></html>\n";
            send_response(client_socket, response);
        }
        else {
            // Check if the file exists and serve it
            std::ifstream file(&path[1], std::ios::binary);
            if (file) {
                std::ostringstream response_stream;
                response_stream << "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
                response_stream << file.rdbuf();
                std::string response = response_stream.str();
                send_response(client_socket, response);
            }
            else {
                // 404 Not Found
                std::string response = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<html><body><h1>404 Not Found</h1></body></html>\n";
                send_response(client_socket, response);
            }
        }
    }
    else {
        // Unsupported method
        std::string response = "HTTP/1.1 501 Not Implemented\nContent-Type: text/html\n\n<html><body><h1>501 Not Implemented</h1></body></html>\n";
        send_response(client_socket, response);
    }
}

int main() {
    WSADATA wsa;
    SOCKET server_fd, client_socket;
    struct sockaddr_in server, client;
    int addrlen = sizeof(server);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cout << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cout << "Socket creation failed." << std::endl;
        return 1;
    }

    // Set address parameters
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind the socket to localhost port 8080
    if (bind(server_fd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cout << "Bind failed." << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cout << "Listen failed." << std::endl;
        return 1;
    }

    std::cout << "Server started. Listening on port " << PORT << "..." << std::endl;

    while (1) {
        // Accept incoming connection
        if ((client_socket = accept(server_fd, (struct sockaddr*)&client, &addrlen)) == INVALID_SOCKET) {
            std::cout << "Accept failed." << std::endl;
            return 1;
        }

        std::cout << "Connection accepted." << std::endl;

        // Handle request
        handle_request(client_socket);

        // Close the connection
        closesocket(client_socket);
        std::cout << "Connection closed." << std::endl;
    }

    closesocket(server_fd);
    WSACleanup();

    return 0;
}
