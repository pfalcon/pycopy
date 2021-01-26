def fun():
    x = 1
    def inner():
        return x
    return inner

try:
    fun.__name__
except AttributeError:
    print("SKIP")
    raise SystemExit

print(fun().__name__)
