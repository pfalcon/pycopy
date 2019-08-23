def foo():
    return 1


var1 = None


def __main__():
    print("in __main__")
    global foo, var1

    try:
        foo = 1
    except RuntimeError:
        print("RuntimeError1")

    def foo2():
        return 2

    try:
        foo = foo2
    except RuntimeError:
        print("RuntimeError2")

    var1 = foo
    print(var1())
    var1 = foo2
    print(var1())
