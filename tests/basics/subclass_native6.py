# Calling native base class unbound method with subclass instance.

class mylist(list):
    pass


l = mylist((1, 2, 3))
assert type(l) is mylist
print(l)

list.append(l, 4)
print(l)
