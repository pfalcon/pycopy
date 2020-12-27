# Related to import handling overriding matters - ensure that we can execute
# a function in module's context, to initialize module. This emulates
# execution of module top-level code in module context.
import sys


mod = type(sys)("")


def init_mod():
    global var
    var = 123


exec(init_mod, mod)
print(mod.var)
print("var" in globals())
