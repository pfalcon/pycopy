class Foo:
    pass


Foo.var = 1


def __main__():
    print("in __main__")
    # del Foo.var

    Foo.var = 2
    try:
        Foo.var2 = 2
    except RuntimeError:
        print("RuntimeError1")
