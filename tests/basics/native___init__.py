# CPython __init__ behavior for native types is all over the place.

# For tuples, can pass any args, and __init__ doesn't affect the object
# (that's expected, it's immutable after all; but any args?)
t = (1, 2, 3)
t.__init__((4, 5, 6))
print(t)
t.__init__(("able", 2, "pass", any, "crap"))
print(t)


# Wow, with lists, it finally makes sense. __init__ is just old good init
# method! You can use it to (re)init object inplace.
l = [1, 2, 3]
l.__init__([4, 5, 6])
print(l)


# Oops, but with dicts, it's not a old good init method! It's something
# completely different, an alias for update() method!
d = {1: 2, 3: 4}
d.__init__({5: 6, 7: 8})
print(sorted(d))
