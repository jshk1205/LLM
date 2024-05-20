#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <WS2tcpip.h> // For inet_ntop function

#define _CRT_SECURE_NO_WARNINGS

#define PORT 53
#define BUFFER_SIZE 1024

// Structure to represent a DNS query
typedef struct {
    unsigned short id;
    unsigned short flags;
    unsigned short qdcount;
    unsigned short ancount;
    unsigned short nscount;
    unsigned short arcount;
    char* qname;
} DNSQuery;

// Function to parse a DNS query packet
DNSQuery* parse_dns_query(char* buffer, int buflen) {
    DNSQuery* query = (DNSQuery*)malloc(sizeof(DNSQuery));
    if (!query) {
        return NULL;
    }
    memset(query, 0, sizeof(DNSQuery));

    // Extract ID from the query
    query->id = (buffer[0] << 8) | buffer[1];

    // Extract flags from the query
    query->flags = (buffer[2] << 8) | buffer[3];

    // Extract QDCOUNT (Question Count) from the query
    query->qdcount = (buffer[4] << 8) | buffer[5];

    // Extract ANCOUNT (Answer Record Count) from the query
    query->ancount = (buffer[6] << 8) | buffer[7];

    // Extract NSCOUNT (Authority Record Count) from the query
    query->nscount = (buffer[8] << 8) | buffer[9];

    // Extract ARCOUNT (Additional Record Count) from the query
    query->arcount = (buffer[10] << 8) | buffer[11];

    // Extract QNAME from the query
    int pos = 12; // Start position of QNAME
    int len;
    char domain[256] = ""; // Maximum domain name length
    while ((len = buffer[pos]) != 0) {
        if (pos != 12) {
            strcat_s(domain, sizeof(domain), ".");
        }
        strncat_s(domain, sizeof(domain), &buffer[pos + 1], len);
        pos += len + 1;
    }
    query->qname = _strdup(domain); // Use _strdup instead of strdup


    return query;
}

// Function to generate DNS response for a given query
char* generate_dns_response(DNSQuery* query) {
    // Construct DNS response packet
    // For simplicity, we'll construct a basic response with hardcoded values

    char* response = (char*)malloc(1024); // Allocate memory for response
    if (!response) {
        return NULL;
    }
    memset(response, 0, 1024);

    // Copy ID from query to response
    response[0] = (query->id >> 8) & 0xFF;
    response[1] = query->id & 0xFF;

    // Set flags (for simplicity, assume standard response with no errors)
    response[2] = (1 << 7); // QR (Response) bit set to 1
    response[3] = 0; // No error (RCODE = 0)

    // Set QDCOUNT, ANCOUNT, NSCOUNT, and ARCOUNT to 0
    response[4] = 0;
    response[5] = 1; // Responding with one answer
    response[6] = 0;
    response[7] = 0;

    // Copy QNAME from query to response
    int pos = 12; // Start position of QNAME in response
    char* qname = query->qname;
    int qname_len = strlen(qname);
    response[pos++] = qname_len;
    memcpy(&response[pos], qname, qname_len);
    pos += qname_len;

    // Set QTYPE to A (IPv4 address)
    response[pos++] = 0;
    response[pos++] = 1;

    // Set QCLASS to IN (Internet)
    response[pos++] = 0;
    response[pos++] = 1;

    // Set TTL (Time to Live) to 1 second
    response[pos++] = 0;
    response[pos++] = 0;
    response[pos++] = 0;
    response[pos++] = 1;

    // Set RDLENGTH (Resource Data Length) to 4 bytes (IPv4 address)
    response[pos++] = 0;
    response[pos++] = 4;

    // Set RDATA (Resource Data) to an IPv4 address (e.g., 192.168.1.1)
    char ip_address[] = { 192, 168, 1, 1 };
    memcpy(&response[pos], ip_address, 4);
    pos += 4;

    return response;
}

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server, client;
    int c, recv_len;
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
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return 1;
    }
    puts("Bind done");

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while (1) {
        fflush(stdout);

        if ((recv_len = recvfrom(s, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client, &c)) == SOCKET_ERROR) {
            printf("recvfrom() failed with error code : %d", WSAGetLastError());
            return 1;
        }

        printf("Received packet from %s:%d\n", inet_ntop(AF_INET, &(client.sin_addr), buffer, INET_ADDRSTRLEN), ntohs(client.sin_port)); // Use inet_ntop instead of inet_ntoa
        printf("Data: %s\n", buffer);

        // Parse DNS query
        DNSQuery* query = parse_dns_query(buffer, recv_len);
        if (!query) {
            printf("Failed to parse DNS query\n");
            continue;
        }

        // Generate DNS response
        char* response = generate_dns_response(query);
        if (!response) {
            printf("Failed to generate DNS response\n");
            continue;
        }

        // Reply to the client
        if (sendto(s, response, strlen(response), 0, (struct sockaddr*)&client, sizeof(client)) == SOCKET_ERROR) {
            printf("sendto() failed with error code : %d", WSAGetLastError());
            return
                1;
        }

        // Free memory
        free(query->qname);
        free(query);
        free(response);
    }

    closesocket(s);
    WSACleanup();

    return 0;
}
