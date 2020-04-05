try:
    import uio
except ImportError:
    print('SKIP')
    raise SystemExit

try:
    b = uio.StringIO()
    b += "foo"
except TypeError:
    print('SKIP')
    raise SystemExit

buf = uio.StringIO()
buf += "foo"
buf += "bar"
print(buf.getvalue())

buf = uio.BytesIO()
buf += b"foo"
buf += b"bar"
print(buf.getvalue())
