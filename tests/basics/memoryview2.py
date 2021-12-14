# test memoryview with arrays and accessing maximum values for signed/unsigned elements
try:
    from uarray import array
except ImportError:
    try:
        from array import array
    except ImportError:
        print("SKIP")
        raise SystemExit

try:
    memoryview
except:
    print("SKIP")
    raise SystemExit

a = array('i', [1, 2, 3, 4])
m = memoryview(a)
print(list(m))
print(list(m[1:-1]))
m[2] = 6
print(a)

print(list(memoryview(b'\x7f\x80\x81\xff')))
print(list(memoryview(array('b', [0x7f, -0x80]))))
print(list(memoryview(array('B', [0x7f, 0x80, 0x81, 0xff]))))
print(list(memoryview(array('h', [0x7f00, -0x8000]))))
print(list(memoryview(array('H', [0x7f00, 0x8000, 0x8100, 0xffff]))))
