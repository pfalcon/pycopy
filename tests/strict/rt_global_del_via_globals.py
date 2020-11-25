# It's not possible to delete a global var at runtime in strict mode.

gvar = 1
del gvar
gvar = 2


def __main__():
    print("in __main__")
    global gvar

    # In the current implementation, TypeError is thrown. This is considered
    # an implementation detail and may change later to e.g. RuntimeError.
    try:
        del globals()["gvar"]
    except:
        print("Exception1")

    print(gvar)
