class mylist(list):
    pass


l = mylist((1, 2, 3))
assert type(l) is mylist
print(l)
l.__init__((4, 5, 6))
print(l)
list.__init__(l, (7, 8, 9))
print(l)

print("---")


class mylist(list):

    def __new__(cls, *args):
        o = list.__new__(cls)
        o.append("beg")
        print("__new__", o)
        return o


l = mylist([1, 2, 3])
assert type(l) is mylist
print(l)

print("---")

class mylist(list):

    def __init__(self, l):
        super().__init__(l + ["end"])


l = mylist([1, 2, 3])
assert type(l) is mylist
print(l)

print("---")

class mylist(list):

    def __new__(cls, *args):
        o = list.__new__(cls)
        o.append("beg")
        print("__new__", o)
        return o

    def __init__(self, l):
        super().__init__(l + ["end"])


l = mylist([1, 2, 3])
assert type(l) is mylist
print(l)

print("---")

class mylist(list):

    def __new__(cls, *args):
        o = super().__new__(cls)
        o.append("beg")
        print("__new__", o)
        return o

    def __init__(self, l):
        list.__init__(self, l + ["end"])


l = mylist([1, 2, 3])
assert type(l) is mylist
print(l)

