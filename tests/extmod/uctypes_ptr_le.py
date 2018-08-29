import sys
try:
    import uctypes
except ImportError:
    print("SKIP")
    raise SystemExit

if sys.byteorder != "little":
    print("SKIP")
    raise SystemExit

desc = {
    "ptr": (uctypes.PTR | 0, uctypes.UINT8),
    "ptr16": (uctypes.PTR | 0, uctypes.UINT16),
    "ptr2": (uctypes.PTR | 0, {"b": uctypes.UINT8 | 0}),
}

bts = b"01"

addr = uctypes.addressof(bts)
buf = addr.to_bytes(uctypes.sizeof(desc, uctypes.LITTLE_ENDIAN), "little")

S = uctypes.struct(uctypes.addressof(buf), desc, uctypes.LITTLE_ENDIAN)

print(addr == int(S.ptr))
print(addr == int(S.ptr2))

print(S.ptr[0])
assert S.ptr[0] == ord("0")
print(S.ptr[1])
assert S.ptr[1] == ord("1")
print(hex(S.ptr16[0]))
assert hex(S.ptr16[0]) == "0x3130"
print(S.ptr2[0].b, S.ptr2[1].b)
print (S.ptr2[0].b, S.ptr2[1].b)
print(hex(S.ptr16[0]))
assert (S.ptr2[0].b, S.ptr2[1].b) == (48, 49)

# Check that can assign address to a pointer
S.ptr = uctypes.addressof(b"23")
print(S.ptr[0])
assert S.ptr[0] == ord("2")
print(S.ptr[1])
assert S.ptr[1] == ord("3")

# Check that can assign one pointer to another
buf2 = bytearray(uctypes.sizeof(desc, uctypes.LITTLE_ENDIAN))
S2 = uctypes.struct(uctypes.addressof(buf2), desc, uctypes.LITTLE_ENDIAN)
S2.ptr = S.ptr
print(hex(S2.ptr16[0]))
print(bytes(S2.ptr))
