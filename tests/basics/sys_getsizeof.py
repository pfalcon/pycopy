# test sys.getsizeof() function
# note - float test is in ../float/

import sys
try:
    sys.getsizeof
except AttributeError:
    print('SKIP')
    raise SystemExit

print(sys.getsizeof(1) >= 2)
print(sys.getsizeof("") >= 2)
print(sys.getsizeof((1, 2)) >= 2)
print(sys.getsizeof([1, 2]) >= 2)
print(sys.getsizeof({1: 2}) >= 2)

class A:
    pass
print(sys.getsizeof(A()) > 0)


try:
    assert sys.getsizeof(set()) >= 2
except NameError:
    pass

# Only test deque if we have it
try:
    from ucollections import deque
    assert sys.getsizeof(deque((), 1)) > 0
except ImportError:
    pass
