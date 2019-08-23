class Foo:
    pass


Foo.var = 1


def __main__():
    print("in __main__")

    try:
        del Foo.var
    except RuntimeError:
        print("RuntimeError1")

    print(Foo.var)
