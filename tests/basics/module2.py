# uPy behaviour only: builtin modules are read-only
import micropython
try:
    micropython.x = 1
except AttributeError:
    print("AttributeError")
