"""
doc

string"""

try:
    __doc__
except NameError:
    print("SKIP")
    raise SystemExit

print(__doc__)
