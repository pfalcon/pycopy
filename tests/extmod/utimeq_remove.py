try:
    from utimeq import utimeq
except ImportError:
    print("SKIP")
    raise SystemExit

SIZE = 7

DEBUG = 0

if DEBUG:
    def dprint(*v):
        print(*v)
else:
    def dprint(*v):
        pass

h = utimeq(SIZE)

def push(v):
    return h.push(v, None, None)

def pop():
    l = [0, 0, 0]
    h.pop(l)
    return l[0]

def expect_IndexError(idx, case_id):
    try:
        h.remove(idx)
        assert False
    except IndexError:
        pass
        dprint(case_id + ": IndexError")

def test_invariants(last_id):
    dprint("---")
    expect_IndexError(-1, "rem neg")
    expect_IndexError(SIZE, "rem overlen")
    for i in range(last_id + 1, SIZE):
        expect_IndexError(i, "rem non-ex%d" % i)

expect_IndexError(0, "rem empty")

id5 = push(5)
test_invariants(id5)
id4 = push(4)
test_invariants(id4)
id3 = push(3)
test_invariants(id3)
id2 = push(2)
test_invariants(id2)
id1 = push(1)
test_invariants(id1)

assert len(h) == 5

#print(id3)
h.remove(id3)
expect_IndexError(id3, "rem already rem'd")
id3_new = push(6)
assert id3_new == id3
h.remove(id3)

l = []
try:
    while 1:
        l.append(pop())
except IndexError:
    pass

dprint(l)
assert len(l) == 4

print("OK")
