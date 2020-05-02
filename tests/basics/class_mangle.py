class A:
    __attr = 1
if "__attr" in dir(A):
    print("SKIP")
    raise SystemExit


class Cls:

    __attr = 1
    __attr += 2
    attr = 100
    attr += __attr

    def __bar(self):
        print("in __bar")
        self.__foo()

    def __foo(self):
        print("in __foo")

    bar = __bar


o = Cls()
try:
    o.__bar()
except AttributeError:
    print("AttributeError1")
o.bar()

try:
    print(Cls.__attr)
except AttributeError:
    print("AttributeError2")

print(Cls.attr)
print(Cls._Cls__attr)
