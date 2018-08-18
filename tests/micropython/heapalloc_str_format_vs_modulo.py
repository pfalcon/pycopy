import micropython

def main():
    print('"%s" % "a"')
    t = micropython.mem_total()
    "%s" % "a"
    mem_modulo = micropython.mem_total() - t
    #print(mem_modulo)

    print('"{}".format("a")')
    t = micropython.mem_total()
    "{}".format("a")
    mem_format = micropython.mem_total() - t
    #print(mem_format)
    print(mem_format <= mem_modulo)


    print('"%d" % 1')
    t = micropython.mem_total()
    "%d" % 1
    mem_modulo = micropython.mem_total() - t
    #print(mem_modulo)

    print('"{}".format(1)')
    t = micropython.mem_total()
    "{}".format(1)
    mem_format_int = micropython.mem_total() - t
    #print(mem_format_int)
    print(mem_format_int <= mem_modulo)


    print('"{:d}".format(1)')
    t = micropython.mem_total()
    "{:d}".format(1)
    mem_format = micropython.mem_total() - t
    #print(mem_format)
    print(mem_format <= mem_modulo)

    print('"%s" % 1')
    t = micropython.mem_total()
    "%s" % 1
    mem_modulo = micropython.mem_total() - t
    #print(mem_modulo)
    print(mem_format_int < mem_modulo)

    # Multiple args should be always more efficient with .format(), as that
    # avoids allocating arg tuple.
    print('"%s %d" % ("a", 1)')
    t = micropython.mem_total()
    "%s %d" % ("a", 1)
    mem_modulo = micropython.mem_total() - t
    #print(mem_modulo)

    print('"{} {}".format("a", 1)')
    t = micropython.mem_total()
    "{} {}".format("a", 1)
    mem_format_2arg = micropython.mem_total() - t
    #print(mem_format_2arg)
    print(mem_format_2arg < mem_modulo)

    # ... But preallocated tuple, should be the same.
    print('"%s %d" % ("a", 1)  #2')
    args = ("a", 1)
    t = micropython.mem_total()
    "%s %d" % args
    mem_modulo = micropython.mem_total() - t
    #print(mem_modulo)
    print(mem_format_2arg == mem_modulo)

main()
