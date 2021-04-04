class mytuple(tuple):
    pass


t = mytuple((1, 2, 3))
assert type(t) is mytuple
print(t)
# Ignored.
t.__init__((4, 5, 6))
print(t)
t.__init__("can", 1, "pass", any, b"crap")
print(t)


class mytuple(tuple):

    def __new__(cls, v):
        return tuple.__new__(cls, ("beg",) + v + ("end",))


t = mytuple((1, 2, 3))
assert type(t) is mytuple
print(t)


class mytuple(tuple):

    def __new__(cls, v):
        return super().__new__(cls, ("beg",) + v + ("end",))

t = mytuple((1, 2, 3))
assert type(t) is mytuple
print(t)
