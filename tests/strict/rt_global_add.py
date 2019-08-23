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

    try:
        gvar2 = 2
    except RuntimeError:
        print("RuntimeError1")
