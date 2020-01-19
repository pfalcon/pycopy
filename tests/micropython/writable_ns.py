import micropython

try:
    micropython.writable_ns
except AttributeError:
    print("SKIP")
    raise SystemExit

try:
    str.foometh = lambda x: x + "!"
except AttributeError:
    print("expected AttributeError #1")

micropython.writable_ns(str, True)

# Patch existing method - this should not relocate namespace dict, so
# check that default version is writable.
str.isalpha = lambda x: x + "!"
print("foo".isalpha())

# Now add new method
str.foometh = lambda x: x + "?"

micropython.writable_ns(str, False)

print("foo".isalpha())
print("bar".foometh())

try:
    str.foometh = lambda x: x + "!"
except AttributeError:
    print("expected AttributeError #2")


# Now check that patching sys preserve handling of sys.stdout, etc. overriding
import sys

micropython.writable_ns(sys, True)
sys.foo = 123
# overriding sys.stdout wouldn't work as expected while namespace is writable
micropython.writable_ns(sys, False)

import uio

buf = uio.StringIO()
org = sys.stdout
sys.stdout = buf
print("Shouldn't be printed on its own")

sys.stdout = org
print("Captured: %r" % buf.getvalue())
