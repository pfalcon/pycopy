try:
    import uio as io
except ImportError:
    try:
        import io
    except ImportError:
        print('SKIP')
        raise SystemExit


buf = io.BytesIO(b"line1\nline2\nline3\nline4")

print(buf.readline())
print(buf.readline(-1))
print(buf.readline(2))
print(buf.readline())
print(buf.readline())
print(buf.readline())
