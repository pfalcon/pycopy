import sys


mod_type = type(sys)

try:
    mod1 = mod_type("sys")
except TypeError:
    print("SKIP")
    raise SystemExit

print(mod1)

# Should be a new module, not existing sys or something
print(len(dir(mod1)) < 6)

mod1.var = 123
print(mod1.var)

# Should be a new module again
mod2 = mod_type("sys")
print(mod1 is mod2)
print(hasattr(mod2, "var"))
mod2.var = 456
print(mod1.var, mod2.var)
