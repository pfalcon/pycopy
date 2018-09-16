# test sys.intern() function

import sys
try:
    sys.intern
except AttributeError:
    print('SKIP')
    raise SystemExit

s1 = "long long long long long long"
s2 = "long long long" + " long long long"

print(id(s1) == id(s2))

i1 = sys.intern(s1)
i2 = sys.intern(s2)

print(id(i1) == id(i2))

i2_ = sys.intern(i2)

print(id(i2_) == id(i2))

try:
    sys.intern(1)
except TypeError:
    print("TypeError")
