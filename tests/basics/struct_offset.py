# Using structs which are not aligned on machine word boundary
try:
    import ustruct as struct
except:
    try:
        import struct
    except ImportError:
        print("SKIP")
        raise SystemExit

buf = struct.pack("I", 0x01020304)
buf_offseted = b"\xff" + buf

v1 = struct.unpack("I", buf)

v2 = struct.unpack_from("I", buf_offseted, 1)
print("1:", v1 == v2)

v3 = struct.unpack("I", memoryview(buf_offseted)[1:])
print("2:", v1 == v3)


wbuf = bytearray(20)
struct.pack_into("I", wbuf, 1, 0x01020304)
print("3:", bytes(wbuf[1:]).startswith(buf))

wbuf = bytearray(20)
struct.pack_into("I", memoryview(wbuf)[1:], 0, 0x01020304)
print("4:", bytes(wbuf[1:]).startswith(buf))
