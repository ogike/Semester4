# server.py localhost 10001 creds.txt

import sys
import socket
import hashlib
import select

server_addr = "localhost"#sys.argv[1]
server_port = int(10001) #int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind((server_addr, server_port))
sock.listen(5)

BUFFER_SIZE = 100
inputs = [sock]

credentials = {}

#reading existing creds from creds.txt
with open(sys.argv[1]) as f:
    file_content = f.readlines()


for line in file_content:
    line = line.rstrip("\n")  # a readlines nem veszi le a sorvégi entereket
    entry = line.split(":")
    credentials[entry[0]] = entry[1]

print("Credentials in the given file:", credentials)

try:
    while True:
        readables, _, _ = select.select(inputs, [], [])

        for s in readables:
            if s is sock:
                connection, client_info = sock.accept()
                print("Csatlakozott valaki: %s:%d" % client_info)
                inputs.append(connection)
            else:
                ## GETTING MSG FROM CLIENT #####################
                msg = s.recv(BUFFER_SIZE)
                if not msg:
                    s.close()
                    print("A kliens lezárta a kapcsolatot")
                    inputs.remove(s)
                    continue

                username, password = msg.decode().split(":")


                password_hasher = hashlib.md5(password.encode())
                password_hash = password_hasher.hexdigest()
                print(password)
                print(password_hash)
                if username in credentials:
                    if credentials[username] == password_hash:
                        answ = "OK"
                    else:
                        answ = "INCORRECT"
                else:
                    credentials[username] = password_hash
                    answ = "CREATED"
                    #TODO: writing to file?
                s.sendall(answ.encode())
                print("Elküldött válasz:", answ)

finally:
    sock.close()
