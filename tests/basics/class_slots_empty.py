try:
    object.__new__
except AttributeError:
    print("SKIP")
    raise SystemExit


class C:
    __slots__ = ()

o = C()
print(type(o).__name__)
