def fun():
    """function
    docstring"""

try:
    fun.__doc__
except AttributeError:
    print("SKIP")
    raise SystemExit

print(fun.__doc__)
