import hashlib

#létre kell hozni hozzá egy md5 objektumot
m = hashlib.md5("Nobody expects".encode())
m.update(b" the spanish inquisition")
print("Digest: {}".format(m.digest()))
print("Digest in hex: 0x{}".format(m.hexdigest()))
print("Digest size: {}".format(m.digest_size))

#részenként feldolgozó hash-elő algo:
another = hashlib.md5(b"Nobody expects the spanish inquisition")
print("\nAnother Digest : {}".format(another.digest()))


yetAnother = hashlib.md5(b"Kecske")
print("\nYet another Digest size: {}".format(yetAnother.digest_size))       #mindig 128 bit lesz
