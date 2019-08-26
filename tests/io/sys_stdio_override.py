try:
    import uio as io
except ImportError:
    try:
        import io
    except ImportError:
        print('SKIP')
        raise SystemExit

import sys
try:
    sys.stdout = sys.stdout
except AttributeError:
    print('SKIP')
    raise SystemExit


buf = io.StringIO()
sys.stdout = buf
print(1, "test", 10 + 20)
