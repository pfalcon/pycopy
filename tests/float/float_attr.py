# fmt: off
try:
    1.1.foo
except AttributeError:
    print("AttributeError")

try:
    1.1e10.foo
except AttributeError:
    print("AttributeError")
