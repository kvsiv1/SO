import socket
import threading
from threading import Lock

clients = []  # List to hold all connected clients
clients_lock = Lock() # lock to synchronize access to clients list

# function to broadcast a message
def broadcast(message, sender_socket):
    with clients_lock:
        for client in clients:
            if client != sender_socket: # don't send to sender
                try:
                    client.sendall(message.encode('utf-8'))
                except:
                    client.close()
                    clients.remove(client)

# function to handle connected client
def handle_client(client_socket, client_address):
    with clients_lock: # add client with lock
        clients.append(client_socket)
    try:
        while True:
            data = client_socket.recv(1024) # receive data from client
            if not data:
                break
            message = data.decode('utf-8')
            print(f"{client_address} sent: {message}") # print received message
            broadcast_msg = f"{client_address} says: {message}"
            broadcast(broadcast_msg, client_socket) # send message to all other clients
    finally:
        client_socket.close()
        with clients_lock: # remove client with lock
            if client_socket in clients:
                clients.remove(client_socket)
        print(f"Connection with {client_address} closed.")

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # create socket
    host = '127.0.0.1'
    port = 12345
    server_socket.bind((host, port))
    server_socket.listen(5) # listen for connections, 5 - max clients to be accepted
    print(f"Server listening on {host}:{port}")

    while True:
        client_socket, client_address = server_socket.accept() # accept new client
        print(f"Accepted connection from {client_address}")
        client_handler = threading.Thread(target=handle_client, args=(client_socket, client_address)) # start a thread to handle client communication
        client_handler.start()

if __name__ == "__main__":
    main()
