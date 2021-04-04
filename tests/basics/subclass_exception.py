class MyExc(Exception):
    pass


e = MyExc(1, 2, 3)
print(e.args)

try:
    e = MyExc(1, b=2)
except TypeError:
    # "function doesn't take keyword arguments"
    pass

print("---1")

# This relies on BaseException.__new__() capturing positional args (and
# ignoring kwargs). This is pretty weird behavior which we faithfully
# implement, but might configurably optimize out.
class MyExc(Exception):

    def __init__(self, a, b, c=1):
        self.a = a


e = MyExc(1, 2)
print(e.args)
print(e.a)

e = MyExc(2, 3, 4)
print(e.args)
print(e.a)

e = MyExc(2, 3, c=4)
print(e.args)
print(e.a)

print("---2")

class MyExc(Exception):

    def __init__(self, a, b, c=1):
        print("__init__", a, b, c)
        self.a = a
        Exception.__init__(self, a, b, c)


e = MyExc(1, 2)
print(e.args)
print(e.a)

e = MyExc(2, 3, 4)
print(e.args)
print(e.a)

e = MyExc(2, 3, c=4)
print(e.args)
print(e.a)

print("---3")

class MyExc(Exception):

    def __init__(self, a, b, c=1):
        print("__init__", a, b, c)
        self.a = a
        super().__init__(a, b, c)

#print("mro:", MyExc.__mro__)
e = MyExc(1, 2)
print(e.args)
print(e.a)

e = MyExc(2, 3, 4)
print(e.args)
print(e.a)

e = MyExc(2, 3, c=4)
print(e.args)
print(e.a)
