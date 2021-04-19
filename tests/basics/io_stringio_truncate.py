try:
    import uio as io
except ImportError:
    try:
        import io
    except ImportError:
        print("SKIP")
        raise SystemExit

# Only truncate at size=0 supported so far.
a = io.StringIO()
a.write("foo")
print(a.getvalue())
print("truncate:", a.truncate(0))
print(repr(a.getvalue()))

# Note - this will write at position 3, as .truncate()
# doesn't update file offset
a.write("bar")
print(a.getvalue())

a.seek(0)
print("truncate:", a.truncate())
print(repr(a.getvalue()))
