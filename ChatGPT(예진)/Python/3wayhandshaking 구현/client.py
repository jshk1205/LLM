import socket


def client_three_way_handshake(server_ip, server_port):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Step 1: 클라이언트가 서버에 SYN 패킷을 보냅니다.
    client_socket.connect((server_ip, server_port))
    client_socket.send(b'SYN')

    # Step 3: 클라이언트는 서버로부터 SYN-ACK 패킷을 받습니다.
    syn_ack = client_socket.recv(1024)
    print("Received SYN-ACK from server:", syn_ack.decode())

    # Step 4: 클라이언트는 서버에 ACK 패킷을 보냅니다.
    client_socket.send(b'ACK')

    # 연결이 설정되었습니다.
    print("Connection established.")
    client_socket.close()


if __name__ == "__main__":
    server_ip = '127.0.0.1'  # 서버의 IP 주소
    server_port = 12345  # 서버의 포트 번호
    client_three_way_handshake(server_ip, server_port)
