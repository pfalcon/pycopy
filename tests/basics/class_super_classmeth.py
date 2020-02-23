class C: pass

try:
    C.__name__
except AttributeError:
    print("SKIP")
    raise SystemExit


class Base:

    @classmethod
    def meth(cls):
        print("in Base:", cls.__name__)
        print("clsattr:", cls.clsattr)


class Derived(Base):

    clsattr = 10

    @classmethod
    def meth(cls):
        print("in Derived:", cls.__name__)
        super(Derived, cls).meth()
        super().meth()


Derived.meth()
