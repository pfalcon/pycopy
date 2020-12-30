from pycopy import icast


print(repr(icast(b"foo", str)))
print(repr(icast(b"", str)))

b1 = b"foobar"
id1 = id(b1)
b2 = icast(b1, str)
id2 = id(b2)
print(id1 == id2)
print(b1)
