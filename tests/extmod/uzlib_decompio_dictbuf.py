# Test passing dictionary buffer to uzlib.DecompIO()
try:
    import uzlib
    import uio as io
except ImportError:
    print("SKIP")
    raise SystemExit

dictbuf = bytearray(512)

# zlib bitstream
inp = uzlib.DecompIO(io.BytesIO(b'\x18\x95\xcbH\xcd\xc9\xc9\xcf\x00\x11\x00\x16\xbc\x04)'), 0, dictbuf)
print(inp.read())

# Just check thar dictbug got some content in it, though it actually
# would contain the output data.
print(dictbuf != bytearray(512))
