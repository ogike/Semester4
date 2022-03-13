from socket import socket, AF_INET, SOCK_STREAM
import sys

server_addr = (sys.argv[1], int(sys.argv[2]))

# automatikus nyitás/bezárás with-el
with socket(AF_INET, SOCK_STREAM) as server:
    server.bind(server_addr)
    server.listen(1)

    client, client_addr = server.accept()
    end = False

    # még egy nem kétező fájlt megnyitok írásra bináris módban:
    with open(sys.argv[3], "wb") as f:
        while not end:
            data = client.recv(128)
            if data:
                f.write(data)
            else:
                client.close()
                end = True
print("Sikerült!")
