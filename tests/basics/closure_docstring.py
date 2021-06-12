def fun():
    x = 1
    def inner():
        """function
        docstring"""
        return x
    return inner

try:
    fun.__doc__
except AttributeError:
    print("SKIP")
    raise SystemExit

print(fun().__doc__)
