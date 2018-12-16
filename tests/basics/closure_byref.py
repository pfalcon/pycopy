# In Python closures, free variables are captured by reference, not value

def test():

    var = 1

    def f():
        print(var)

    def g():
        nonlocal var
        var = 3

    f()
    var = 2
    f()
    g()
    print(var)


test()
