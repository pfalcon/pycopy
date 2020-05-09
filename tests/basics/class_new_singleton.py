try:
    # If we don't expose object.__new__ (small ports), there's
    # nothing to test.
    object.__new__
except AttributeError:
    print("SKIP")
    raise SystemExit


class Singleton:

    obj = None

    def __new__(cls):
        if cls.obj is None:
            cls.obj = super().__new__(cls)
        return cls.obj


s1 = Singleton()
s2 = Singleton()
print(s1 is s2)
print(isinstance(s1, Singleton), isinstance(s2, Singleton))
