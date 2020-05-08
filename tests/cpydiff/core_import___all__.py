"""
categories: Core,import
description: __all__ attribute of modules/packages isn't processed by Pycopy
cause: Pycopy employs simplified model, where ``from mod import *`` imports all names, except those starting with an underscore. This handling is compatible with CPython.
workaround: Don't rely on __all__. For symbols which you don't want to be imported with ``from mod import *``, start their names with ``_``. You can still import such a symbol explicitly with ``from mod import _sym``.
"""
from modules.modall import *

print([name for name in dir() if "foo" in name])
