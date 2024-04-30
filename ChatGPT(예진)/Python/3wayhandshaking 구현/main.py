import client
import server
import threading


def main():
    server_port = 12345

    # 서버를 실행합니다.
    server_thread = threading.Thread(target=server.server_three_way_handshake, args=(server_port,))
    server_thread.start()

    # 클라이언트를 실행합니다.
    client_ip = '127.0.0.1'
    client.client_three_way_handshake(client_ip, server_port)


if __name__ == "__main__":
    main()
