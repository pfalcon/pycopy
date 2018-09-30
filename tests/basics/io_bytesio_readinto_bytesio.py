# Test extension to readinto() - accepting a stream buffer (BytesIO) as an arg
try:
    import uio as io
except ImportError:
    print('SKIP')
    raise SystemExit

src = io.BytesIO(b"string")
dest = io.BytesIO(5)

print(src.readinto(dest, 2))
print(dest.getvalue())
print(dest.seek(0, 1))

print(src.readinto(dest))
print(dest.getvalue())
print(dest.seek(0, 1))

print(src.readinto(dest))
print(dest.getvalue())
print(dest.seek(0, 1))


# Write in the middle of existing data
src = io.BytesIO(b"string")
dest = io.BytesIO(bytearray(b"1234567890"))
dest.seek(2)
print(src.readinto(dest))
print(dest.getvalue())
print(dest.seek(0, 1))
