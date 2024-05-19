#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#define MAX_HOSTS 3
#define TIMEOUT_MS 1000 // 응답 대기 시간 (1초)

// 호스트 정보를 저장하는 구조체
typedef struct {
    const char* ip;
    const char* name;
} HostInfo;

// 스레드 함수
DWORD WINAPI SendICMPRequest(LPVOID lpParam) {
    HostInfo* hostInfo = (HostInfo*)lpParam;

    // 호스트 정보 출력
    printf("Sending ICMP request to %s (%s)...\n", hostInfo->name, hostInfo->ip);

    // 호스트의 DNS 이름 확인
    struct addrinfo* result = NULL;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    HANDLE hIcmpFile = INVALID_HANDLE_VALUE;
    DWORD ipaddr = 0;
    DWORD ReplySize = 0;

 
    LPVOID ReplyBuffer = NULL;
 
    char SendData[] = "Data Buffer";

    int ret = getaddrinfo(hostInfo->ip, NULL, &hints, &result);
    if (ret == 0 && result != NULL) {
        char hostname[NI_MAXHOST];
        ret = getnameinfo(result->ai_addr, (socklen_t)result->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, NI_NAMEREQD);
        if (ret == 0) {
            printf("DNS name for %s: %s\n", hostInfo->ip, hostname);
        }
        else {
            printf("Failed to get DNS name for %s.\n", hostInfo->ip);
        }
        freeaddrinfo(result);
    }
    else {
        printf("Failed to get address info for %s.\n", hostInfo->ip);
    }

    // ICMP 요청 보내기 및 응답 처리
 
    DWORD dwRetVal2 = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, TIMEOUT_MS);
    if (dwRetVal2 != 0) {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
        struct in_addr ReplyAddr;
        ReplyAddr.S_un.S_addr = pEchoReply->Address;
        char addrBuffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ReplyAddr.S_un.S_addr), addrBuffer, INET_ADDRSTRLEN);
        printf("Received response from %s: status = %ld, response time = %lums\n", addrBuffer, pEchoReply->Status, pEchoReply->RoundTripTime);
    }
    else {
        printf("IcmpSendEcho failed: %ld\n", GetLastError());
    }


    return 0;
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    const char* TARGET_HOSTS[MAX_HOSTS] = { "8.8.8.8", "1.1.1.1", "208.67.222.222" };
    const char* HOST_NAMES[MAX_HOSTS] = { "Google DNS", "Cloudflare DNS", "OpenDNS" }; // 호스트의 DNS 이름

    // 호스트 정보 구조체 초기화
    HostInfo hostInfos[MAX_HOSTS];
    for (int i = 0; i < MAX_HOSTS; i++) {
        hostInfos[i].ip = TARGET_HOSTS[i];
        hostInfos[i].name = HOST_NAMES[i];
    }

    // 스레드 핸들 배열
    HANDLE threadHandles[MAX_HOSTS];

    // 각 호스트에 대해 병렬로 스레드를 생성하여 ICMP 요청을 보냄
    for (int i = 0; i < MAX_HOSTS; i++) {
        threadHandles[i] = CreateThread(NULL, 0, SendICMPRequest, &hostInfos[i], 0, NULL);
        if (threadHandles[i] == NULL) {
            printf("Failed to create thread for host %s.\n", hostInfos[i].name);
        }
    }

    // 모든 스레드가 종료될 때까지 대기
    WaitForMultipleObjects(MAX_HOSTS, threadHandles, TRUE, INFINITE);

    // 스레드 핸들 정리
    for (int i = 0; i < MAX_HOSTS; i++) {
        CloseHandle(threadHandles[i]);
    }

    WSACleanup();
    return 0;
}
