import socket
import threading

clients = []  # List to hold all connected clients

def broadcast(message, sender_socket):
    for client in clients:
        if client != sender_socket:
            try:
                client.sendall(message.encode('utf-8'))
            except:
                client.close()
                clients.remove(client)

def handle_client(client_socket, client_address):
    clients.append(client_socket)
    try:
        while True:
            data = client_socket.recv(1024)
            if not data:
                break
            message = data.decode('utf-8')
            print(f"{client_address} sent: {message}")
            broadcast_msg = f"{client_address} says: {message}"
            broadcast(broadcast_msg, client_socket)
    finally:
        client_socket.close()
        if client_socket in clients:
            clients.remove(client_socket)
        print(f"Connection with {client_address} closed.")

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host = '127.0.0.1'
    port = 12345
    server_socket.bind((host, port))
    server_socket.listen(5)
    print(f"Server listening on {host}:{port}")

    while True:
        client_socket, client_address = server_socket.accept()
        print(f"Accepted connection from {client_address}")
        client_handler = threading.Thread(target=handle_client, args=(client_socket, client_address))
        client_handler.start()

if __name__ == "__main__":
    main()
