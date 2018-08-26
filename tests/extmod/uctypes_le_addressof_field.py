try:
    import uctypes
except ImportError:
    print("SKIP")
    raise SystemExit

desc = {
    "f0": uctypes.UINT8 | 0,
    "f1": uctypes.UINT8 | 5,
    "f2": uctypes.UINT8 | 2,
    "f3": (uctypes.PTR | 8, uctypes.UINT8),
    "arr": (uctypes.ARRAY | 3, uctypes.UINT8 | 2),
    "sub": (4, {
        "b0": uctypes.UINT8 | 0,
        "b1": uctypes.UINT8 | 1,
    }),
}

data = bytearray(10)

start_addr = uctypes.addressof(data)

S = uctypes.struct(start_addr, desc, uctypes.LITTLE_ENDIAN)

addr = uctypes.addressof(S, "f0")
print(addr - start_addr)

addr = uctypes.addressof(S, "f1")
print(addr - start_addr)

addr = uctypes.addressof(S, "f2")
print(addr - start_addr)

addr = uctypes.addressof(S, "f3")
print(addr - start_addr)

addr = uctypes.addressof(S, "arr")
print(addr - start_addr)

addr = uctypes.addressof(S, "sub")
print(addr - start_addr)

addr = uctypes.addressof(S.sub, "b0")
print(addr - start_addr)

addr = uctypes.addressof(S.sub, "b1")
print(addr - start_addr)
