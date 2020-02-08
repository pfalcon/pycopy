gl = lambda: (yield)

g = gl()
print(next(g))
try:
    g.send(42)
except StopIteration as e:
    print(e.value)
