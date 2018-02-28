# test MicroPython-specific feature of passing stream to .unpack() function

try:
    import ustruct as struct
    import uio as io
except:
    print("SKIP")
    raise SystemExit

buf = io.BytesIO(b"12")
print(struct.unpack1("B", buf))
print(buf.read())

buf = io.BytesIO(b"123456")
print(struct.unpack1("<H", buf))
print(struct.unpack1("<I", buf))
print(buf.read())

buf = io.BytesIO(b"123456")
print(struct.unpack("<HI", buf))
print(buf.read())

buf = io.BytesIO(b"12")
try:
    print(struct.unpack1("<I", buf))
except EOFError:
    print("EOFError")
# We're at EOF, partially read data lost
print(buf.read())

buf = io.BytesIO(b"12")
print(struct.unpack1("2s", buf))

buf = io.BytesIO(b"12")
try:
    print(struct.unpack1("3s", buf))
except EOFError:
    print("EOFError")

# This is impl-specific limit, may as well work around it
buf = io.BytesIO(b"1" * 64)
try:
    print(struct.unpack1("32s", buf))
except ValueError:
    print("ValueError")
