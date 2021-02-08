try:
    import uio as io
    io.TextIOWrapper
except:
    try:
        import io
        io.TextIOWrapper
    except:
        print('SKIP')
        raise SystemExit


s = io.TextIOWrapper(io.BytesIO("123привет".encode()))
print(s.read(1))
print(s.read(3))
print(s.read(1))
print(s.read())
