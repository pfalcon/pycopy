import mod


def my_foo():
    print("my_foo")


def __main__():
    try:
        mod.foo = my_foo
    except RuntimeError:
        print("RuntimeError1")
