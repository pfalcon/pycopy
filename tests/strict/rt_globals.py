# It's not possible to modify globals via globals() at run-time, only read
# them.

gvar = 1


def __main__():
    print("in __main__")

    print(globals()["gvar"])
    # In the current implementation, TypeError is thrown. This is considered
    # an implementation detail and may change later to e.g. RuntimeError.
    try:
        globals()["gvar"] = 2
    except:
        print("Exception1")

    print(gvar)
