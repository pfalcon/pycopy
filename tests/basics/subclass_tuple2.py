class C(tuple):

    def __new__(cls, tpl, val):
        print("C.__new__")
        obj = tuple.__new__(cls, tpl)
        #print("in new:", type(obj))
        assert type(obj) is C
        obj.val = val
        return obj


o = C((1, 2), 3)
assert type(o) is C
print(o)
print(o.val)

print("--")

class C(tuple):

    def __new__(cls, tpl, val):
        print("C.__new__")
        obj = super().__new__(cls, tpl)
        #print("in new:", type(obj))
        assert type(obj) is C
        obj.val = val
        return obj


o = C((1, 2), 3)
assert type(o) is C
print(o)
print(o.val)
