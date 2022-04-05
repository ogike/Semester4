# client.py localhost 10001 admin 1234 josh alma timothy jelszo
# kimenetek: OK INCORRECT CREATED

import socket
import sys

BUFFER_SIZE = 100
TCP_IP =  "localhost"#sys.argv[1]
TCP_PORT = int(10001)#int(sys.argv[2])
HISTORY_IP = "localhost"
HISTORY_PORT = 10002

# csúnyán néz ki de csak annyit csinál, hogy párosával kiveszi az elemeket a sys.argv-ből a 3-as indextől kezdve és
# összefűzve kettősponttal
credentials = [ ( sys.argv[i] + ":" + sys.argv[i + 1] ) for i in range( 1, len(sys.argv), 2 )]
print("Usernames and passwords to try:", credentials)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.connect((TCP_IP, TCP_PORT))

for entry in credentials:
    #sending login:pwd
    sock.sendall(entry.encode())

    #getting back reply (OK/INCORRECT/CREATED)
    reply = sock.recv(BUFFER_SIZE)
    decoded_reply = reply.decode()
    print("answer:", decoded_reply)

    #sending to history via udp
    to_store = (entry + ":" + decoded_reply).encode()
    udp_sock.sendto(to_store, (HISTORY_IP, HISTORY_PORT))

sock.close()
udp_sock.close()
