try:
    try:
        from ucollections import namedtuple
    except ImportError:
        from collections import namedtuple
except ImportError:
    print("SKIP")
    raise SystemExit


T = namedtuple("T", ())
t = T()
print(t)

# Neither Pycopy nor CPython optimize empty named tuples.
print(T() is T())
