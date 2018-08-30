try:
    import uio as io
except:
    try:
        import io
    except ImportError:
        print('SKIP')
        raise SystemExit

try:
    io.IOBase
except AttributeError:
    print('SKIP')
    raise SystemExit


class MyIO(io.IOBase):
    def write(self, buf):
        # CPython and uPy pass in different types for buf (str vs bytearray)
        print('write', len(buf))
        return len(buf)

print('test', file=MyIO())
