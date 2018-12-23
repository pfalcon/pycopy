try:
    from utimeq import utimeq
except ImportError:
    print("SKIP")
    raise SystemExit

h = utimeq(4)

assert h.push(0, 1, 2) == 0
assert h.push(0, 1, 2) == 1
assert h.push(0, 1, 2) == 2
assert h.push(0, 1, 2) == 3

try:
    h.push(0, 1, 2)
    assert False
except IndexError:
    pass

h.pop([0, 0, 0])
assert h.push(0, 1, 2) == 0

h.pop([0, 0, 0])
assert h.push(0, 1, 2) == 1

h.pop([0, 0, 0])
assert h.push(0, 1, 2) == 2

h.pop([0, 0, 0])
assert h.push(0, 1, 2) == 3

h.pop([0, 0, 0])
assert h.push(0, 1, 2) == 0

print("OK")
