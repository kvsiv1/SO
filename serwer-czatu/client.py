import socket
import threading

# function receives messages continously
def receive_messages(client_socket):
    while True:
        try:
            # try to receive data
            data = client_socket.recv(1024)
            if data:
                # print received message
                print("\n" + data.decode('utf-8') + "\n> ", end="")
            else:
                break
        except:
            break

def main():
    # Create a socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host = '127.0.0.1'
    port = 12345
    client_socket.connect((host, port)) # connect to server

    # Start a thread to listen for messages from the server
    listener = threading.Thread(target=receive_messages, args=(client_socket,), daemon=True)
    listener.start()

    while True: # loop to send messages
        try:
            message = input("> ")
            if message.lower() == 'exit': # exit to quit
                break
            client_socket.sendall(message.encode('utf-8')) # send a message to server
        except KeyboardInterrupt: # ctrl c
            break

    client_socket.close()

if __name__ == "__main__":
    main()
