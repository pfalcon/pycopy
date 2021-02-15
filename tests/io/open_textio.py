try:
    import uos as os
except ImportError:
    import os

if not hasattr(os, "remove"):
    print("SKIP")
    raise SystemExit

try:
    import uio as io

    io.TextIOWrapper
except:
    try:
        import io

        io.TextIOWrapper
    except:
        print("SKIP")
        raise SystemExit


# cleanup in case testfile exists
try:
    os.remove("testfile")
except OSError:
    pass

f1 = open("testfile", "wb")
tf1 = io.TextIOWrapper(f1)
tf1.write("привет")

tf1.flush()

f2 = open("testfile", "rb")
s = f2.read()
f2.close()
assert s == "привет".encode()

tf1.close()

try:
    tf1.write("should fail")
    assert False, "Could write to closed textio wrapper"
except ValueError:
    pass

try:
    f1.write(b"should fail2")
    assert False, "Could write to closed underlying file"
except ValueError:
    pass

# cleanup
try:
    os.remove("testfile")
except OSError:
    pass
