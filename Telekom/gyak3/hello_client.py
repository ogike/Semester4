import socket
import sys
import hashlib

TCP_IP = "localhost"
TCP_PORT = 10000
BUFFER_SIZE = 1024
message = b"hello server"

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#connectelek egy szerverre (bind ellentéte)
sock.connect((TCP_IP, TCP_PORT))
sock.send(message) #bytestring-et küld
#send vs sendall:
    #send: próbálkozás utan ha nem sikerül elérni socketet, kivételt dob
    #sendall: x-szer megpróbálja elküldeni, csak utána kivétel

reply = sock.recv(BUFFER_SIZE) #csak max ennyi mennyiségűt tud olvasni, de kevesebbet is elfogad
                                #akár egy darab eof-ot is
sock.close()

print("Válasz:", reply.decode())