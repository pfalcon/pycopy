# Function named like module.
def mod2():
    print("mod2")


# This module is supposed to be imported, not run directly. But run-tests
# script will run it, so add dummy __main__() and corresponding .exp file.
def __main__():
    print("Shouldn't run")
