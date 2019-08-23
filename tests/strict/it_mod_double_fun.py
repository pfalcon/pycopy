import mod


def foo():
    return 1


try:
    mod.foo = foo
except RuntimeError:
    print("RuntimeError1")

print(mod.foo())

try:
    mod.foo = 1
except RuntimeError:
    print("RuntimeError2")

print(mod.foo)

try:
    mod.foo = 2
except RuntimeError:
    print("RuntimeError3")

print(mod.foo)


def __main__():
    pass
