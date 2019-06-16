# memoryview.init(str)
try:
    memoryview.init
except:
    print("SKIP")
    raise SystemExit

m = memoryview("123")
print(list(m))

# For strings, offset/size are in terms of underlying bytes, not characters
m.init("абвгд", 1, 2)
print(list(m))
