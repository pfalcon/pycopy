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


print(next(g))
print(next(g))
g.pend_throw(ValueError())

v = None
try:
    v = next(g)
except Exception as e:
    print("raised", repr(e))

print("ret was:", v)

# It's legal to pend exception in a just-started generator, just the same
# as it's legal to .throw() into it.
g = gen()
g.pend_throw(ValueError())
try:
    next(g)
except ValueError:
    print("ValueError from just-started gen")
