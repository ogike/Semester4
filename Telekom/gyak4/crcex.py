import binascii
import zlib

test_string = "Fekete retek rettenetes".encode("utf-8")

print(binascii.crc32(bytearray(test_string)))
print(hex(zlib.crc32(test_string)))
