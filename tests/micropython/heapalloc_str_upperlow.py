import micropython

micropython.heap_lock()

print("foo".lower())
print("123foo".lower())

print("FOO".upper())
print("123FOO".upper())

micropython.heap_unlock()
