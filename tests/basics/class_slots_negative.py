# Test error conditions for __slots__ class. These are however not
# errors for non-__slots__ class, and as there's no easy way to
# verify is __slots__ support is enabled, this may fail for ports
# which don't have it enabled.
class C:

        clslvl = 1

        __slots__ = ("value",)


o = C()
o.value = 1
try:
    # Read-only in CPython
    o.clslvl = 2
except AttributeError:
    print("AttributeError")

try:
    o.foo = 2
except AttributeError:
    print("AttributeError")

try:
    o.__dict__
except AttributeError:
    print("AttributeError")
