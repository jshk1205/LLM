#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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

int ping(const char* host, int count, int interval, PingResult* result);
int traceroute(const char* host, std::vector<std::string>& hops);
void save_to_file(const std::string& data, const std::string& filename);

int main() {
    char target[256];
    std::cout << "Enter a host name or IP address: ";
    std::cin >> target;

    struct sockaddr_in sa;
    struct addrinfo* ai;
    int ret;

    if (inet_pton(AF_INET, target, &(sa.sin_addr)) == 1) {
        ret = getnameinfo((struct sockaddr*)&sa, sizeof(sa), target, sizeof(target), NULL, 0, NI_NAMEREQD);
        if (ret == 0) {
            std::cout << "Host name for " << target << ": " << target << std::endl;
        }
        else {
            std::cout << "Failed to resolve host name for " << target << std::endl;
        }
    }
    else {
        ret = getaddrinfo(target, NULL, NULL, &ai);
        if (ret == 0) {
            std::cout << "IP address for " << target << ": " << inet_ntoa(((struct sockaddr_in*)(ai->ai_addr))->sin_addr) << std::endl;
        }
        else {
            std::cout << "Failed to resolve IP address for " << target << std::endl;
        }
        freeaddrinfo(ai);
    }

    int count = 10;
    int interval = 1;
    PingResult ping_result;
    if (ping(target, count, interval, &ping_result) == 0) {
        std::cout << "Avg response time: " << ping_result.avg_response_time << " ms" << std::endl;
        std::cout << "Packet loss rate: " << ping_result.packet_loss_rate << "%" << std::endl;

        std::string ping_data = "Avg response time: " + std::to_string(ping_result.avg_response_time) + " ms\n" +
            "Packet loss rate: " + std::to_string(ping_result.packet_loss_rate) + "%";
        save_to_file(ping_data, "ping_result.txt");

        std::cout << "Response times:" << std::endl;
        for (int i = 0; i < ping_result.response_count; ++i) {
            std::cout << ping_result.response_times[i] << " ms" << std::endl;
        }
    }
    else {
        std::cout << "Failed to ping " << target << std::endl;
    }

    std::vector<std::string> hops;
    int hop_count = traceroute(target, hops);
    if (hop_count >= 0) {
        std::cout << "Traceroute Results:" << std::endl;
        for (int i = 0; i < hop_count; ++i) {
            std::cout << hops[i] << std::endl;
        }
        std::string traceroute_data = hops[0] + "\n";
        for (int i = 1; i < hop_count; ++i) {
            traceroute_data += hops[i] + "\n";
        }
        save_to_file(traceroute_data, "traceroute_result.txt");
    }
    else {
        std::cout << "Failed to traceroute to " << target << std::endl;
    }

    return 0;
}

int ping(const char* host, int count, int interval, PingResult* result) {
    char command[100];
    snprintf(command, sizeof(command), "ping -n %d -w %d %s", count, interval * 1000, host);

    FILE* ping_output = _popen(command, "r");
    if (ping_output == nullptr) {
        return -1;
    }

    float response_times[MAX_HOPS];
    int response_count = 0;
    char line[MAX_RESPONSE_LEN];
    while (fgets(line, sizeof(line), ping_output) != nullptr) {
        if (strstr(line, "time=") != nullptr) {
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

int traceroute(const char* host, std::vector<std::string>& hops) {
    char command[100];
    snprintf(command, sizeof(command), "tracert %s", host);

    FILE* traceroute_output = _popen(command, "r");
    if (traceroute_output == nullptr) {
        return -1;
    }

    int hop_count = 0;
    char line[MAX_RESPONSE_LEN];
    while (fgets(line, sizeof(line), traceroute_output) != nullptr) {
        std::string hop(line);
        hops.push_back(hop);
        hop_count++;
    }

    _pclose(traceroute_output);
    return hop_count;
}

void save_to_file(const std::string& data, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << data;
        file.close();
    }
}
