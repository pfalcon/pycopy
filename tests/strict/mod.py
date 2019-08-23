print(__name__)
val = 1


def foo():
    print("foo")


# This module is supposed to be imported, not run directly. But run-tests
# script will run it, so add dummy __main__() and corresponding .exp file.
def __main__():
    print("Shouldn't run")
