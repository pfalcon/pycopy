try:
    b"".__class__ = str
except AttributeError:
    print("SKIP")
    raise SystemExit


b = b"foo"
print(b)
b.__class__ = str
print(b)
print(b == "foo")

b = b""
print(b)
b.__class__ = str
print(repr(b))
print(b == "")

# This returns R/O mp_const_empty_bytes
b = bytes()
print(b)
try:
    b.__class__ = str
    assert 0, "Shouldn't be possible to change R/O object"
except AttributeError:
    print("AttributeError")
