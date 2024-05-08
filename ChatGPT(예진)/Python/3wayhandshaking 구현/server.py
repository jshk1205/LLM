import socket


def server_three_way_handshake(server_port):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', server_port))
    server_socket.listen(1)

    print("Server is listening on port", server_port)

    while True:
        # 클라이언트의 연결 요청을 기다립니다.
        client_socket, client_address = server_socket.accept()

        # Step 2: 서버는 클라이언트로부터 SYN 패킷을 받고, SYN-ACK 패킷을 보냅니다.
        syn_packet = client_socket.recv(1024)
        print("Received SYN from client:", syn_packet.decode())
        client_socket.send(b'SYN-ACK')

        # Step 5: 서버는 클라이언트로부터 ACK 패킷을 받습니다.
        ack_packet = client_socket.recv(1024)
        print("Received ACK from client:", ack_packet.decode())

        # 연결이 설정되었습니다.
        print("Connection established with", client_address)
        client_socket.close()


if __name__ == "__main__":
    server_port = 12345  # 서버의 포트 번호
    server_three_way_handshake(server_port)
