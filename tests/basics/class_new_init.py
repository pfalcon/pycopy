# Lookup of __new__ and __init__ methods in various ways.
try:
    # If we don't expose object.__new__ (small ports), there's
    # nothing to test.
    object.__new__
except AttributeError:
    print("SKIP")
    raise SystemExit


# Base class doesn't define __new__/__init__, effectively inheriting
# them from "object".
class A:
    pass


class B(A):

    def __init__(self):
        A.__init__(self)
        self.fld = "B_fld"

    @classmethod
    def new(cls):
        self = A.__new__(cls)
        self.fld = "B_new_fld"
        return self

class C(A):

    def __init__(self):
        super().__init__()
        self.fld = "C_fld"

    @classmethod
    def new(cls):
        self = super().__new__(cls)
        self.fld = "C_new_fld"
        return self

o1 = B()
print(type(o1).__name__, o1.fld)
o2 = C()
print(type(o2).__name__, o2.fld)

o3 = B.new()
print(type(o3).__name__, o3.fld)
o4 = C.new()
print(type(o4).__name__, o4.fld)


# Now base class defines __new__/__init__
class A1:

    def __new__(cls):
        return object.__new__(cls)

    def __init__(self):
        pass


class B1(A1):

    def __init__(self):
        A1.__init__(self)
        self.fld = "B1_fld"

    @classmethod
    def new(cls):
        self = A1.__new__(cls)
        self.fld = "B1_new_fld"
        return self


class C1(A1):

    def __init__(self):
        super().__init__()
        self.fld = "C1_fld"

    @classmethod
    def new(cls):
        self = super().__new__(cls)
        self.fld = "C1_new_fld"
        return self


o5 = B1()
print(type(o5).__name__, o5.fld)
o6 = C1()
print(type(o6).__name__, o6.fld)

o7 = B1.new()
print(type(o7).__name__, o7.fld)
o8 = C1.new()
print(type(o8).__name__, o8.fld)
