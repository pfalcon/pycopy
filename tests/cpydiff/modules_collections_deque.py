"""
categories: Modules,collections
description: Only minimal deque functionality is implemented
cause: Builtin deques are intended for efficient, zero-allocation queuing, and thus are fixed-size and provide only ``.append()`` and ``.popleft()`` methods.
workaround: Use regular lists. pycopy-lib provides full implementation of collections.deque.
"""
import collections
D = collections.deque()
print(D)
