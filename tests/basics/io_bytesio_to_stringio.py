try:
    import uio
    uio.TextIOBase
except:
    print('SKIP')
    raise SystemExit


s = uio.TextIOBase(uio.BytesIO(b"123привет"))
print(s.read(1))
print(s.read(3))
print(s.read(1))
print(s.read())
