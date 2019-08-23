v = 1

try:

    def v():
        return 1


except RuntimeError:
    print("RuntimeError1")


def __main__():
    pass
