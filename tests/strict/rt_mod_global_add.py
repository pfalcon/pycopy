# It's not possible to create a new attribute (global var) in another module
# at runtime in strict mode (all globals should be pre-defined).

import mod

mod.bar = "mine"
print(mod.bar)
del mod.bar
mod.bar = "mine2"

print(mod.bar)


def __main__():
    print("in __main__")
    # del mod.bar
    mod.bar = "mine3"
    print(mod.bar)

    try:
        mod.bar2 = "mine2"
    except RuntimeError:
        print("RuntimeError1")
