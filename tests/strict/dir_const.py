var1 = 1

const1: const = 2


def foo():
    pass


class Class:
    def meth1(self):
        pass


print(sorted([x for x in dir() if x != "extra_coverage"]))

# Extra stuff in pycopy-dev, can't print directly.
dirl = dir(Class)
assert "meth1" in dirl


def __main__():
    pass
