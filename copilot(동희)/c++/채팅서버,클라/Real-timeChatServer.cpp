// 해당 코드는 우분투에서 만들려고해
// 리눅스 환경에서 실행할꺼야
// ChatServer를 만들려고해
// C++언어로 만들려고해
// ChatServer는 다음과 같은 기능을 가지고 있어야해
// 소켓 프로그래밍: 서버 소켓 생성, 바인딩, 리스닝, 클라이언트 연결 수락
// 멀티스레딩: 여러 클라이언트의 동시 접속을 처리하기 위한 스레드 생성 및 관리
// 클라이언트 관리: 연결된 클라이언트 소켓을 저장하고 관리
// 메시지 브로드캐스트: 한 클라이언트가 보낸 메시지를 다른 모든 클라이언트에게 전송
// 로그 및 에러 처리: 서버의 활동과 에러를 로그로 기록
// 신호 처리: 서버 종료 시 자원을 적절히 해제하는 기능
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>

#define PORT 12345

std::vector<int> client_sockets; // 연결된 클라이언트 소켓 관리

void broadcastMessage(const std::string& message, int sender_socket) {
    std::string fullMessage = message + "\n"; // 메시지 끝에 종료 문자 추가
    for (int client_socket : client_sockets) {
        if (client_socket != sender_socket) { // 메시지를 보낸 클라이언트 제외
            ssize_t bytes_sent = send(client_socket, fullMessage.c_str(), fullMessage.size(), 0);
            if (bytes_sent == -1) {
                std::cerr << "Error sending message to client socket " << client_socket << ": " << strerror(errno) << std::endl;
            } else {
                std::cout << "Sent " << bytes_sent << " bytes to client socket " << client_socket << std::endl;
            }
        }
    }
}

void handleClient(int client_socket) {
    std::string welcomeMessage = "서버로부터: 환영합니다!\n";
    send(client_socket, welcomeMessage.c_str(), welcomeMessage.size(), 0);

    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            std::cerr << "Connection closed or error occurred.\n";
            break; // 클라이언트 연결 종료 처리
        }
        std::cout << "클라이언트로부터: " << buffer << std::endl;

        // 메시지 브로드캐스트
        broadcastMessage(std::string(buffer, bytes_received), client_socket);
    }

    // 클라이언트와의 연결 종료
    close(client_socket);
    client_sockets.erase(std::remove(client_sockets.begin(), client_sockets.end(), client_socket), client_sockets.end());
}

int startServer() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Socket creation failed.\n";
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Binding failed.\n";
        close(server_socket);
        return -1;
    }

    if (listen(server_socket, 5) == -1) {
        std::cerr << "Listening failed.\n";
        close(server_socket);
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
        if (client_socket == -1) {
            std::cerr << "Accepting client connection failed.\n";
            continue; // 연결 실패 시 다음 연결 시도 대기
        }

        client_sockets.push_back(client_socket); // 클라이언트 소켓 저장

        std::thread client_thread(handleClient, client_socket);
        client_thread.detach(); // 클라이언트 스레드 분리
    }

    close(server_socket);
}

int main() {
    startServer();
    return 0;
}
