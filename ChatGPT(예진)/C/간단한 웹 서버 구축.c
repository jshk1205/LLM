#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

void send_response(SOCKET client_socket, const char* response) {
    send(client_socket, response, strlen(response), 0);
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
            char* response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Welcome to the Home Page!</h1></body></html>\n";
            send_response(client_socket, response);
        }
        else if (strcmp(path, "/about") == 0) {
            // About page
            char* response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>About Us</h1><p>This is the about page.</p></body></html>\n";
            send_response(client_socket, response);
        }
        else {
            // Check if the file exists and serve it
            FILE* file = fopen(&path[1], "rb");
            if (file) {
                fseek(file, 0, SEEK_END);
                long file_size = ftell(file);
                fseek(file, 0, SEEK_SET);
                char* response = (char*)malloc(file_size + strlen("HTTP/1.1 200 OK\nContent-Type: text/html\n\n") + 1);
                strcpy(response, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
                fread(response + strlen(response), file_size, 1, file);
                fclose(file);
                send_response(client_socket, response);
                free(response);
            }
            else {
                // 404 Not Found
                char* response = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<html><body><h1>404 Not Found</h1></body></html>\n";
                send_response(client_socket, response);
            }
        }
    }
    else {
        // Unsupported method
        char* response = "HTTP/1.1 501 Not Implemented\nContent-Type: text/html\n\n<html><body><h1>501 Not Implemented</h1></body></html>\n";
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
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        return 1;
    }

    // Set address parameters
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind the socket to localhost port 8080
    if (bind(server_fd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        return 1;
    }

    printf("Server started. Listening on port %d...\n", PORT);

    while (1) {
        // Accept incoming connection
        if ((client_socket = accept(server_fd, (struct sockaddr*)&client, &addrlen)) == INVALID_SOCKET) {
            printf("Accept failed.\n");
            return 1;
        }

        printf("Connection accepted.\n");

        // Handle request
        handle_request(client_socket);

        // Close the connection
        closesocket(client_socket);
        printf("Connection closed.\n");
    }

    closesocket(server_fd);
    WSACleanup();

    return 0;
}
