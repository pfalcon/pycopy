# It's not possible to delete a global var at runtime in strict mode.

gvar = 1
del gvar
gvar = 2


def __main__():
    print("in __main__")
    global gvar

    try:
        del gvar
    except RuntimeError:
        print("RuntimeError1")

    print(gvar)
