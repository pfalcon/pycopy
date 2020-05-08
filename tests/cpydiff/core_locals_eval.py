"""
categories: Core,Runtime
description: Code running in eval() function doesn't have access to local variables
cause: Pycopy doesn't maintain symbolic local environment, it is optimized to an array of slots. Thus, local variables can't be accessed by a name. Effectively, ``eval(expr)`` in Pycopy is equivalent to ``eval(expr, globals(), globals())``.
workaround: Unknown
"""
val = 1


def test():
    val = 2
    print(val)
    eval("print(val)")


test()
