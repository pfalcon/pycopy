# case where generator doesn't intercept the thrown/injected exception
def gen():
    yield 123
    yield 456
        
g = gen()
print(next(g))
try:
    g.throw(KeyError)
except KeyError:
    print('got KeyError from downstream!')

# case where a thrown exception is caught and stops the generator
def gen():
    try:
        yield 1
        yield 2
    except:
        pass
g = gen()
print(next(g))
try:
    g.throw(ValueError)
except StopIteration:
    print('got StopIteration')

# generator ignores a thrown GeneratorExit (this is allowed)
def gen():
    try:
        yield 123
    except GeneratorExit as e:
        print('GeneratorExit', repr(e.args))
    yield 456
        
# thrown a class
g = gen()
print(next(g))
print(g.throw(GeneratorExit))

# thrown an instance
g = gen()
print(next(g))
print(g.throw(GeneratorExit()))

# thrown an instance with None as second arg
g = gen()
print(next(g))
print(g.throw(GeneratorExit(), None))

# thrown a class and instance
g = gen()
print(next(g))
print(g.throw(GeneratorExit, GeneratorExit(123)))

# it's legal to throw exception into just-started generator
g = gen()
try:
    g.throw(KeyError)
except KeyError:
    print('just-started throw: KeyError')

# reinsure that such just-started generator is properly stopped after
# exception
try:
    next(g)
except StopIteration:
    pass

# throwing into stopped generator should raise thrown exception
# (case for exception type)
try:
    g.throw(ValueError)
except ValueError:
    print("ValueError1")

# throwing into stopped generator should raise thrown exception
# (case for exception instance)
try:
    g.throw(ValueError())
except ValueError:
    print("ValueError2")
