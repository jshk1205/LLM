# Description: Server for the messenger application. It listens for two clients to connect and then forwards messages between them.
import socket
import threading
from db_utils import save_message

def handle_client(client_socket, client_address, other_client_socket):
    while True:
        try:
            message = client_socket.recv(1024).decode()
            if message:
                print(f"Received from {client_address}: {message}")
                save_message(str(client_address), "other_client", message)
                other_client_socket.send(message.encode())
            else:
                break
        except:
            break
    client_socket.close()

def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', 12345))
    server.listen(2)
    print("Server started on port 12345")

    client1, addr1 = server.accept()
    print(f"Client 1 connected: {addr1}")
    client2, addr2 = server.accept()
    print(f"Client 2 connected: {addr2}")

    threading.Thread(target=handle_client, args=(client1, addr1, client2)).start()
    threading.Thread(target=handle_client, args=(client2, addr2, client1)).start()

if __name__ == "__main__":
    main()
