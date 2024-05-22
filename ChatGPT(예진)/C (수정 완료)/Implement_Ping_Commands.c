#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") 

#define MAX_HOPS 64
#define MAX_RESPONSE_LEN 1024

struct PingResult {
    float avg_response_time;
    float packet_loss_rate;
    float response_times[MAX_HOPS];
    int response_count;
};

int ping(char* host, int count, int interval, struct PingResult* result);
int traceroute(char* host, char* hops[]);
void save_to_file(char* data, char* filename);

int main() {
    char target[256];
    printf("Enter a host name or IP address: ");
    scanf("%255s", target);

    struct sockaddr_in sa;
    struct addrinfo* ai;
    int ret;

    if (inet_pton(AF_INET, target, &(sa.sin_addr)) == 1) {
        ret = getnameinfo((struct sockaddr*)&sa, sizeof(sa), target, sizeof(target), NULL, 0, NI_NAMEREQD);
        if (ret == 0) {
            printf("Host name for %s: %s\n", target, target);
        }
        else {
            printf("Failed to resolve host name for %s\n", target);
        }
    }
    else {
        ret = getaddrinfo(target, NULL, NULL, &ai);
        if (ret == 0) {
            printf("IP address for %s: %s\n", target, inet_ntoa(((struct sockaddr_in*)(ai->ai_addr))->sin_addr));
        }
        else {
            printf("Failed to resolve IP address for %s\n", target);
        }
        freeaddrinfo(ai);
    }

    int count = 10;
    int interval = 1;
    struct PingResult ping_result;
    if (ping(target, count, interval, &ping_result) == 0) {
        printf("Avg response time: %.2f ms\n", ping_result.avg_response_time);
        printf("Packet loss rate: %.2f%%\n", ping_result.packet_loss_rate);

        char ping_data[100];
        sprintf(ping_data, "Avg response time: %.2f ms\nPacket loss rate: %.2f%%", ping_result.avg_response_time, ping_result.packet_loss_rate);
        save_to_file(ping_data, "ping_result.txt");

        printf("Response times:\n");
        for (int i = 0; i < ping_result.response_count; ++i) {
            printf("%.2f ms\n", ping_result.response_times[i]);
        }
    }
    else {
        printf("Failed to ping %s\n", target);
    }

    char* hops[MAX_HOPS];
    int hop_count = traceroute(target, hops);
    if (hop_count >= 0) {
        printf("Traceroute Results:\n");
        for (int i = 0; i < hop_count; ++i) {
            printf("%s\n", hops[i]);
        }
        char traceroute_data[MAX_RESPONSE_LEN * MAX_HOPS];
        sprintf(traceroute_data, "%s\n", hops[0]);
        for (int i = 1; i < hop_count; ++i) {
            strcat(traceroute_data, hops[i]);
            strcat(traceroute_data, "\n");
        }
        save_to_file(traceroute_data, "traceroute_result.txt");
    }
    else {
        printf("Failed to traceroute to %s\n", target);
    }

    return 0;
}

int ping(char* host, int count, int interval, struct PingResult* result) {
    char command[100];
    snprintf(command, sizeof(command), "ping -n %d -w %d %s", count, interval * 1000, host);

    FILE* ping_output = _popen(command, "r");
    if (ping_output == NULL) {
        return -1;
    }

    float response_times[MAX_HOPS];
    int response_count = 0;
    char line[MAX_RESPONSE_LEN];
    while (fgets(line, sizeof(line), ping_output) != NULL) {
        if (strstr(line, "time=") != NULL) {
            char* time_str = strstr(line, "time=") + 5;
            float time = atof(time_str);
            response_times[response_count++] = time;
        }
    }

    _pclose(ping_output);

    if (response_count > 0) {
        float total_time = 0;
        for (int i = 0; i < response_count; ++i) {
            total_time += response_times[i];
        }
        result->avg_response_time = total_time / response_count;
        result->packet_loss_rate = (1 - (float)response_count / count) * 100;
        result->response_count = response_count;
        memcpy(result->response_times, response_times, sizeof(float) * response_count);
        return 0;
    }
    else {
        return -1;
    }
}

int traceroute(char* host, char* hops[]) {
    char command[100];
    snprintf(command, sizeof(command), "tracert %s", host);

    FILE* traceroute_output = _popen(command, "r");
    if (traceroute_output == NULL) {
        return -1;
    }

    int hop_count = 0;
    char line[MAX_RESPONSE_LEN];
    while (fgets(line, sizeof(line), traceroute_output) != NULL) {
        char* hop = strdup(line);
        if (hop != NULL) {
            hops[hop_count++] = hop;
        }
    }

    _pclose(traceroute_output);
    return hop_count;
}

void save_to_file(char* data, char* filename) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        fputs(data, file);
        fclose(file);
    }
}
