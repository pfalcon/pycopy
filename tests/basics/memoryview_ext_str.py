# MicroPython allows to init memoryview with a string, which allows to
# access its underlying utf-8 data.
try:
    memoryview
except:
    print("SKIP")
    raise SystemExit

m = memoryview("абвг")
print(len(m))
print(m[0], m[-1])
