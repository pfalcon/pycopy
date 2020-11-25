# It's not possible to create a new global var at runtime in strict mode
# (all globals should be pre-defined).

gvar = 1
print("gvar", gvar)


def __main__():
    print("in __main__")
    global gvar, gvar2

    print("gvar", gvar)
    gvar = 2
    print("gvar", gvar)

    # In the current implementation, TypeError is thrown. This is considered
    # an implementation detail and may change later to e.g. RuntimeError.
    try:
        globals()["gvar2"] = 2
    except:
        print("Exception1")
