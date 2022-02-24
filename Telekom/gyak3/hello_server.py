import socket
#import sys
import hashlib

#konstansok
TCP_IP = 'localhost'
TCP_PORT = 10000 #ez egy arbitary random port
BUFFER_SIZE = 1024
reply = b'Hello kliens'     #b = reply.encode()
        #b: byte-okként legyen eltárolva

#socket osztálynak az inicializátora
    #AD_INET: IPV_4 használata
    #SOCK_STREAM: TCP használata
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

#bind-oljuk a megadott ip-címre és portra, ezek leszünk mi
    #tupleként adjuk át
sock.bind((TCP_IP, TCP_PORT))
#elkezdjük figyelni a forgalmat
    #paraméter: hány klienst leszek hajlandó kiszolgálni
sock.listen(1)

#ha valaki csatlakozik, visszakapokkét objektumot (tupleként)
    #conn: socket connection object
    #addr: tuple
conn, addr = sock.accept()
print('Valaki csatlakozott:', addr)


while True:
    #fogadok a klienstől ennyi byteot
    data = conn.recv(BUFFER_SIZE)
    if not data: #ha üres (eof karaktert kaptunk, ami megegyezésre kapcsolat bontást jelent)
	    break
    print("Üzenet:", data.decode()) #bytestring-et kapunk, tehát ezt visszakapjuk
    conn.send(reply)
conn.close()
#sock.close()