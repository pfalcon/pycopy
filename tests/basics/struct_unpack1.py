# test MicroPython-specific .unpack1() function

try:
    import ustruct as struct
except:
    print("SKIP")
    raise SystemExit

print(struct.unpack1("B", b"1"))

try:
    struct.unpack1("B", b"")
except ValueError:
    print("ValueError")

try:
    struct.unpack1("H", b"1")
except ValueError:
    print("ValueError")

try:
    struct.unpack1("2B", b"1")
except ValueError:
    print("ValueError")

print(struct.unpack1("2s", b"12"))

try:
    struct.unpack1("3s", b"12")
except ValueError:
    print("ValueError")
