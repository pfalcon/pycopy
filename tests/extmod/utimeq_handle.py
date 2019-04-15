try:
    from utimeq import utimeq
except ImportError:
    print("SKIP")
    raise SystemExit

h = utimeq(4)

assert h.push(0, 1, 2) == 0
assert h.push(1, 1, 2) == 1
assert h.push(2, 1, 2) == 2
assert h.push(3, 1, 2) == 3

try:
    h.push(0, 1, 2)
    assert False
except IndexError:
    pass

h.pop([0, 0, 0])
assert h.push(4, 1, 2) == 0

h.pop([0, 0, 0])
assert h.push(5, 1, 2) == 1

h.pop([0, 0, 0])
assert h.push(6, 1, 2) == 2

h.pop([0, 0, 0])
assert h.push(7, 1, 2) == 3

h.pop([0, 0, 0])
assert h.push(8, 1, 2) == 0

print("OK")
