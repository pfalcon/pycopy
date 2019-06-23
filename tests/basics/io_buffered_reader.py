try:
    import uio as io
except ImportError:
    try:
        import io
    except ImportError:
        print('SKIP')
        raise SystemExit

try:
    io.BytesIO
    io.BufferedReader
except AttributeError:
    print('SKIP')
    raise SystemExit

BUF_SZ = 4

bts = io.BytesIO()
bts.write(bytes(range(256)))
bts.seek(0)
print(bts.getvalue())

buf = io.BufferedReader(bts, BUF_SZ)
print(bts.seek(0, 1))

print(buf.read(2))
print(bts.seek(0, 1))

print(buf.read(1))
print(bts.seek(0, 1))

print(buf.read(2))
print(bts.seek(0, 1))

print(buf.read(BUF_SZ * 2))
print(bts.seek(0, 1))
