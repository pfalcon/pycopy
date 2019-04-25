# test handling of failed heap allocation with bytearray

import micropython

class GetSlice:
    def __getitem__(self, idx):
        return idx
sl = GetSlice()[:]

# extend bytearray using slice subscr
# depends on MICROPY_PY_ARRAY_SLICE_ASSIGN
b = bytearray(4)
micropython.heap_lock()
try:
    b[sl] = b'01234567'
except MemoryError:
    print('MemoryError: bytearray subscr grow')
micropython.heap_unlock()
print(b)
