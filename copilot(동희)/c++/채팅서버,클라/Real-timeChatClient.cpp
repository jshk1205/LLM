// 해당 코드는 우분투에서 만들려고해
// 리눅스 환경에서 실행할꺼야
// ChatClient를 만들려고해
// C++언어로 만들려고해
// ChatClient는 다음과 같은 기능을 가지고 있어야해
// 소켓 프로그래밍: 서버에 연결하기 위한 클라이언트 소켓 생성 및 연결
// 멀티스레딩: 사용자 입력과 서버로부터의 메시지 수신을 동시에 처리하기 위한 스레드 생성 및 관리
// 메시지 송수신: 사용자 입력을 받아 서버로 전송
// 메시지 송수신: 서버로부터 받은 메시지를 출력
#include <iostream>
#include <string>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring> 

#define PORT 12345
#define IP "127.0.0.1"

void receiveMessages(int client_socket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            std::cerr << "Connection closed or error occurred.\n";
            break; // 서버 연결 종료 처리
        }
        buffer[bytes_received] = '\0'; // Null-terminate the received string
        std::cout << "서버로부터: " << buffer << std::endl;
    }
}

int main() {
    // 서버에 연결하기 위한 클라이언트 소켓 생성
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Socket creation failed.\n";
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &server_address.sin_addr);

    // 서버에 연결 시도
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Connection to server failed.\n";
        close(client_socket);
        return -1;
    } else {
        std::cout << "Successfully connected to the server.\n";
    }

    // 사용자 입력과 서버로부터의 메시지 수신을 동시에 처리하기 위한 스레드 생성
    std::thread receive_thread(receiveMessages, client_socket);

    // 사용자 입력을 받아 서버로 전송
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message == "exit") {
            break; // 사용자가 exit 입력 시 연결 종료
        }
        message += "\n"; // 메시지 끝에 개행 문자 추가
        send(client_socket, message.c_str(), message.size(), 0);
    }

    // 서버와의 연결 종료 및 스레드 정리
    close(client_socket);
    if (receive_thread.joinable()) {
        receive_thread.join(); // 메시지 수신 스레드가 종료될 때까지 대기
    }

    return 0;
}
