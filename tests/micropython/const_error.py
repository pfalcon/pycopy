# make sure syntax error works correctly for bad const definition

from micropython import const


def test_syntax(code):
    try:
        exec(code)
    except SyntaxError:
        print("SyntaxError")


# argument not a constant
# Pycopy: there's absolutely nothing wrong with passing arbitrary expression
# to const(). It's a contract that name on the left side is always bound to
# argument of const(), and is not rebound to any other value. In C terms,
# 'a = const(x)' means 'const a = x'. With variable annotations, there would
# be better syntax: 'a: const = x' (or in MyPy terms, 'a: Final = x'). But
# variable annotations are currently not supported, and need to get general
# syntax/semantics of const() right anyway.

# test_syntax("a = const(x)")

# redefined constant
test_syntax("A = const(1); A = const(2)")
