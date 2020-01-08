# Interaction of .pend_throw() with .throw() and .close()

def gen():
    i = 0
    while 1:
        yield i
        i += 1

g = gen()

try:
    g.pend_throw
except AttributeError:
    print("SKIP")
    raise SystemExit

g.pend_throw(1)
try:
    g.throw(ValueError())
except ValueError:
    print("expected ValueError #1")

g = gen()
print(next(g))
g.pend_throw(2)
try:
    g.throw(ValueError())
except ValueError:
    print("expected ValueError #2")

g = gen()
g.pend_throw(3)
g.close()

g = gen()
print(next(g))
g.pend_throw(4)
g.close()

print("OK")
