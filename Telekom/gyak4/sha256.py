import hashlib

m = hashlib.sha256("Nobody expects".encode())
m.update(b" the spanish inquisition")
print("Digest: {}".format(m.digest()))
print("Digest in hex: 0x{}".format(m.hexdigest()))
print("Digest size: {}".format(m.digest_size))

another = hashlib.sha256(b"Nobody expects the spanish inquisition")
print("\nAnother Digest : {}".format(another.digest()))

yetAnother = hashlib.sha256(b"Kecske")
print("\nYet another Digest size: {}".format(yetAnother.digest_size))       #mindig 256 bit lesz
