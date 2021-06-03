try:
    import uarray as array
except ImportError:
    try:
        import array
    except ImportError:
        print("SKIP")
        raise SystemExit


print(bytearray(b"123") == b"123")
print(bytearray(b"123") == array.array("B", b"123"))
# Despite equalities above, the below are False
print(array.array("B", b"123") == b"123")
print(b"123" == array.array("B", b"123"))
