# test sys.getsizeof() function

import sys
try:
    sys.getsizeof
except AttributeError:
    print('SKIP')
    raise SystemExit

print(sys.getsizeof(1.0) >= 2)
