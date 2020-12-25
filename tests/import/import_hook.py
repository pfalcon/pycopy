import sys

try:
    sys.setimphook
except AttributeError:
    print("SKIP")
    raise SystemExit


class Mod:
    var = 123


def hook(modname, path):
    if path.startswith("*"):
        return None
    print(modname, path.split("/")[-2:])
    return Mod


sys.setimphook(hook, (".ext",))


import notexists1

print(notexists1)
print(notexists1.var)

print("---")
import notexists1

print(notexists1.var)

# Verify that we get a chance to process package's __init__.py.
import pkg
