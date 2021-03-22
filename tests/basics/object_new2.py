# Various ways to call object.__new__
try:
    # If we don't expose object.__new__ (small ports), there's
    # nothing to test.
    object.__new__
except AttributeError:
    print("SKIP")
    raise SystemExit


try:
    object.__new__(int)
except TypeError:
    print("TypeError1")

try:
    object.__new__(int, 1)
except TypeError:
    print("TypeError2")

o = object()
print(type(o))

try:
    object(1)
except TypeError:
    print("TypeError3")

class A: pass

o2 = object.__new__(A)
print(type(o2).__name__)

# TODO: disabled for now, try to reinstate later.
#try:
#    object.__new__(A, 1)
#except TypeError:
#    print("TypeError4")


class B:
    field = 1

o3 = object.__new__(B)
assert "field" in dir(o3)


class C:
    def __init__(self, a, b):
        1/0
        print(a, b)

# CPython allows this, silently ignores extra params in this case
# (i.e. when __init__ is defined).
o4 = object.__new__(C, 1, 2, 3)
print(type(o4).__name__)


class D:
    pass
    def __new__(self, a, b):
        print(a, b)

# TODO: disabled for now, try to reinstate later.
#try:
#    o5 = object.__new__(D, 1, 2)
#except TypeError:
#    print("TypeError5")
