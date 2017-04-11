from socket import *

ADDR = "192.0.2.1"

s1 = socket(AF_INET, SOCK_DGRAM)
s1.bind((ADDR, 55555))

s2 = socket(AF_INET, SOCK_DGRAM)
s2.connect((ADDR, 55555))

data = b"test1234" * 314

sent_len = s2.send(data)
#print(sent_len)
assert sent_len < len(data)
res = s1.recv(10000)
#print(res)
assert res == data[:sent_len]
