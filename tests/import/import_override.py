import import1b
assert import1b.var == 123

import builtins
org_import = builtins.__import__

def my_import(*args):
    # MicroPython currently doesn't pass globals/locals, so don't print them
    # CPython3.5 and lower for "from pkg.mod import foo" appear to call
    # __import__ twice - once with 5 args, and once with 1 (for "pkg")
    # CPython3.5 and MicroPython doesn't have such an artifact. So, to make
    # test pass on CPython3.5-, just don't print calls with less than 5 args
    if len(args) == 5:
        print("overriden import:", args[0], args[3], args[4])
    return org_import(*args)

try:
    builtins.__import__ = my_import
except AttributeError:
    print("SKIP")
    raise SystemExit

# __import__ is called unconditionally on import, even if module is already
# imported (actually, runtime doesn't know or care if module is already
# imported, sys.modules caching is completely on the level of __import__
# itself).
import import1b
print(import1b.var)

from pkg.mod import foo
