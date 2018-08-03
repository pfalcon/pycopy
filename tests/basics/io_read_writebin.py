# This tests extended (MicroPython-specific) methods .readbin(), .writebin()
import uio

try:
    uio.BytesIO
except AttributeError:
    print('SKIP')
    raise SystemExit

buf = uio.BytesIO(b"\xff\xff0123456789abc")

print(buf.readbin("b"))
print(buf.readbin("B"))
print(hex(buf.readbin("<H")))
print(hex(buf.readbin(">H")))
print(hex(buf.readbin("<Q")))

# Check that if there're less bytes left than needed by format, EOFError is raised
try:
    print(hex(buf.readbin("<H")))
except EOFError:
    print("EOFError1")

buf = uio.BytesIO(b"")
# Check that reading from stream at EOF leads to EOFError
try:
    print(buf.readbin("B"))
except EOFError:
    print("EOFError2")


buf = uio.BytesIO()
buf.writebin("b", -128)
buf.writebin("<H", 0x0102)
buf.writebin(">H", 0x0304)
buf.writebin(">Q", 0x05060708090a0b0c)
print(buf.getvalue())
