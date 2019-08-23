# It's not possible to delete an attribute (global var) in another module
# at runtime in strict mode.

import mod

mod.bar = "mine"
del mod.bar
mod.bar = "mine2"


def __main__():
    print("in __main__")
    try:
        del mod.bar
    except RuntimeError:
        print("RuntimeError1")

    print(mod.bar)
