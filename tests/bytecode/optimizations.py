# cmdline: -v -v
# fmt: off

def if_const():
    if 1:
        a1
    if 0:
        a2
    if (1):
        a3
    if (0):
        a4
    if ((1)):
        a5
    if ((0)):
        a6
    if 1 + 2:
        a7
    if 1 - 1:
        a8
    if (1 + 2):
        a9
    if 1 - 1:
        a10


def if_parens():
    if a:
        b1
    if (a):
        b2
    if ((a)):
        b3


# Was "optimized" previously, but was removed due to unlikely giving any
# useful benefit, but taking its code size.
def if_tuple():
    if ():
        a1
    if (1,):
        a2
    if (1, 2):
        a3
