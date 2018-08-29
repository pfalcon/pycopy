class Cud():

    def __int__(self):
        print("__int__ called")
        return -100


cud1 = Cud()

try:
    +cud1
except TypeError:
    print("SKIP")
    raise SystemExit

# the following require MICROPY_PY_ALL_SPECIAL_METHODS
int(cud1)

class BadInt:
    def __int__(self):
        print("__int__ called")

try:
    int(BadInt())
except TypeError:
    print("TypeError")
