# Test conversion of user-defined args to integers when passed to FFI
# functions.
try:
    import ffi
except ImportError:
    print("SKIP")
    raise SystemExit


def ffi_open(names):
    err = None
    for n in names:
        try:
            mod = ffi.open(n)
            return mod
        except OSError as e:
            err = e
    raise err

libc = ffi_open(('libc.so', 'libc.so.0', 'libc.so.6', 'libc.dylib'))

try:
    printf = libc.func("i", "printf", "si")
except OSError:
    print("SKIP")
    raise SystemExit


printf("%d\n", 10)

class Num:

    def __init__(self, v):
        self.v = v

    def __int__(self):
        return self.v

printf("%d\n", Num(12345))
