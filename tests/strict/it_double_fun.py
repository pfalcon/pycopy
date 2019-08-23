v = 1

v = 2

print(v)


def foo():
    return 1


try:

    def foo():
        return 2


except RuntimeError:
    print("RuntimeError1")

print(foo())


def __main__():
    pass
