from import4 import *


g = globals()

if "_sym1" not in g:
    # Assume __all__ is not supported. Perform sanity check before
    # before finishing with SKIP.
    assert "sym2" in g
    assert "sym3" in g
    print("SKIP")
    raise SystemExit
else:
    print(_sym1)
    print(sym2)
    print("sym3" in g)
