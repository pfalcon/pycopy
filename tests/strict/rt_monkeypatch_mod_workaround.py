import mod

# Turn const slot into variable slot, so it can be freely
# assigned to at run-time.
mod.foo = mod.foo


def my_foo():
    print("my_foo")


def __main__():
    mod.foo()
    mod.foo = my_foo
    mod.foo()
