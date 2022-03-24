import socket
import struct
import sys

message = 'Hello group'.encode()
multicast_group = ('224.3.29.71', 10000)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #multicast csak UDP-vel!

# Set the time-to-live for messages to 1 so they do not go past the local network segment.
sock.settimeout(1)

ttl = struct.pack('b', 1)
sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, ttl)
try:

    # Send data to the multicast group
    print('sending {}'.format(message))
    sock.sendto(message, multicast_group)

    # Look for responses from all recipients
    while True:
        print('waiting to receive')
        try:
            data, address = sock.recvfrom(16)
        except socket.timeout:
            print('timed out')
        else:
            print('received {} from {}'.format(data, address))
            break

finally:
    print('closing socket')
    sock.close()