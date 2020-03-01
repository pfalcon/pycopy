# MicroPython compiler incorrectly performed lookups for class variables
# in some cases, conflating class and non-class vars.

def fun1():
    bar = 1

    class Temp:
        bar = 30

    Temp.bar = bar

    return Temp


Cls = fun1()
print(Cls.bar)

o = Cls()
print(o.bar)


def fun2():
    __module__ = 2

    class Temp:
        # __module__ is assigned implicitly
        pass

    Temp.__module__ = __module__

    return Temp


Cls = fun2()
print(Cls.__module__)

o = Cls()
print(o.__module__)
