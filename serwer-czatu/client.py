import socket
import threading

def receive_messages(client_socket):
    while True:
        try:
            data = client_socket.recv(1024)
            if data:
                print("\n" + data.decode('utf-8') + "\n> ", end="")
            else:
                break
        except:
            break

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host = '127.0.0.1'
    port = 12345
    client_socket.connect((host, port))

    # Start a thread to listen for messages from the server
    listener = threading.Thread(target=receive_messages, args=(client_socket,), daemon=True)
    listener.start()

    while True:
        try:
            message = input("> ")
            if message.lower() == 'exit':
                break
            client_socket.sendall(message.encode('utf-8'))
        except KeyboardInterrupt:
            break

    client_socket.close()

if __name__ == "__main__":
    main()
