from socket import *

ADDR = "192.0.2.1"

s1 = socket(AF_INET, SOCK_DGRAM)
s1.bind((ADDR, 55555))

s2 = socket(AF_INET, SOCK_DGRAM)
s2.connect((ADDR, 55555))
s2.send(b"test")
res = s1.recv(1000)
print(res)
assert res == b"test"

s2.send(b"test1234" * 64)
res = s1.recv(1000)
assert res == b"test1234" * 64
