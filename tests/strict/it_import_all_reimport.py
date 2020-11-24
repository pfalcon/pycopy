from mod import *

# mod2 also has "from mod import *", so all symbols will be reimported.
# We check that no (useless) warnings are given in this case.
from mod2 import *


def __main__():
    pass
