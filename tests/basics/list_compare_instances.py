# Test that comparisons of instance sequences use exactly the operation
# specified on instances themselves.

class A:

    def __lt__(self, other):
        print("A.__lt__")
        return True

class B:

    def __gt__(self, other):
        print("B.__gt__")
        return True


print([A()] < [A()])

print([B()] > [B()])
