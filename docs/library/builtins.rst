Builtin types, functions and exceptions (:mod:`builtins`)
=========================================================

All builtin functions and exceptions are described here. They are also
available via ``builtins`` module.

Functions and types
-------------------

.. function:: abs()

.. function:: all()

.. function:: any()

.. function:: bin()

.. class:: bool()

.. class:: bytearray()

.. class:: bytes()

    |see_cpython| `python:bytes`.

.. function:: callable()

.. function:: chr()

.. function:: classmethod()

.. function:: compile()

.. class:: complex()

.. function:: delattr(obj, name)

   The argument *name* should be a string, and this function deletes the named
   attribute from the object given by *obj*.

.. class:: dict()

.. function:: dir()

.. function:: divmod()

.. function:: enumerate()

.. function:: eval()

.. function:: exec()

.. function:: filter()

.. class:: float()

.. class:: frozenset()

.. function:: getattr()

.. function:: globals()

.. function:: hasattr()

.. function:: hash()

.. function:: hex()

.. function:: id()

.. function:: input()

.. class:: int()

   .. classmethod:: from_bytes(bytes, byteorder)

      In Pycopy, `byteorder` parameter must be positional (this is
      compatible with CPython).

   .. method:: to_bytes(size, byteorder)

      In Pycopy, `byteorder` parameter must be positional (this is
      compatible with CPython).

.. function:: isinstance()

.. function:: issubclass()

.. function:: iter()

.. function:: len()

.. class:: list()

.. function:: locals()

.. function:: map()

.. function:: max()

.. class:: memoryview(buffer)
           memoryview(buffer, offset, size)

   |see_cpython| `python:memoryview`.

   .. admonition:: Pycopy extension
      :class: note

      A `str` object can be passed to memoryview() constructor, in which
      case underlying UTF-8 encoded data can be accessed by memoryview.
      ``memoryview("abc")`` is equivalent to ``memoryview("abc".encode())``,
      but is more memory-efficient.

   .. admonition:: Pycopy extension
      :class: note

      Additional *offset* and *size* parameters can be passed to constructor.
      ``mv = memoryview(buf, offset, size)`` is equivalent to
      ``mv1 = memoryview(buf); mv = mv1[offset:offset + size]``, but is more
      memory-efficient.

   .. method:: init(buffer[, offset, size])

      Reinitialize existing `memoryview` object inplace with a new buffer,
      and optionally given *offset* and *size* into it.

      .. admonition:: Pycopy extension
         :class: note

         This method is a Pycopy extension. It doesn't require memory
         allocation. Just as the constructor, this method can be called
         for `str` objects too.

   Example of usage of Pycopy extensions::

    # A function with zero memory allocation which can write either
    # bytes or str objects to a stream, in the presense of short writes.

    # Pre-allocate static memoryview buffer
    bufptr = memoryview(b"")

    def write_any(stream, data):
        bufptr.init(data)
        # We need size in bytes, not in characters in case of Unicode
        # strings, and size of memoryview gives us that.
        sz = len(bufptr)
        offset = 0
        while sz:
            # We use another Pycopy extension, 3-argument stream
            # .write() method. We could use memoryview.init() method
            # to update "visible" part of underlying buffer instead.
            written = stream.write(bufptr, offset, sz)
            offset += written
            sz -= written

.. function:: min()

.. function:: next()

.. class:: object()

.. function:: oct()

.. function:: open()

.. function:: ord()

.. function:: pow()

.. function:: print()

.. function:: property()

.. function:: range()

.. function:: repr()

.. function:: reversed()

.. function:: round()

.. class:: set()

.. function:: setattr()

.. class:: slice()

   The *slice* builtin is the type that slice objects have.

.. function:: sorted()

.. function:: staticmethod()

.. class:: str()

    In Pycopy, strings are backed by Unicode UTF-8 encoding. While this
    fact doesn't have to be known by normal users, advanced users may use it
    to optimize applications/libraries.

.. function:: sum()

.. function:: super()

.. class:: tuple()

.. function:: type()

.. function:: zip()


Exceptions
----------

.. exception:: AssertionError

.. exception:: AttributeError

.. exception:: Exception

.. exception:: ImportError

.. exception:: IndexError

.. exception:: KeyboardInterrupt

.. exception:: KeyError

.. exception:: MemoryError

.. exception:: NameError

.. exception:: NotImplementedError

.. exception:: OSError

    Represents system-level error, related to the operating system or hardware.
    |see_cpython| `python:OSError` for general information.

    Pycopy doesn't implement ``errno`` attribute, instead use the standard way
    to access exception arguments: ``exc.args[0]`` (this is compatible with
    CPython).

    Pycopy doesn't implement subclasses of this exception for specific error
    codes. E.g., instead of `python:TimeoutError`, ``OSError(ETIMEDOUT)``
    is used directly (this is compatible with CPython).

    Pycopy also uses this exception in some cases where instead of standard
    system `errno` code, a different error code is provided (e.g. a more
    detailed/domain specific code). In this case, OSErrno arguments are a tuple
    of size 2, where first argument is an error code, and 2nd is first string,
    representing namespace of the error code. Whenever possible, error codes
    for such namespaced errors should be negative. This allows to easily
    distinguish errors in the default `errno` namespace from namespaced
    errors - checking ``exc.args[0]`` is then enough to distinguish system
    error from namespaced error, and then ``exc.args[1]`` can be consulted
    to get the namespace. Example of namespaced OSError from
    `usocket.getaddrinfo()` function::

        >>> usocket.getaddrinfo("-doesnt-exist-", 80)
        Traceback (most recent call last):
          File "<stdin>", line 1, in <module>
        OSError: (-3, 'gai')

.. exception:: RuntimeError

.. exception:: StopIteration

.. exception:: SyntaxError

.. exception:: SystemExit

    |see_cpython| `python:SystemExit`.

.. exception:: TypeError

    |see_cpython| `python:TypeError`.

.. exception:: ValueError

.. exception:: ZeroDivisionError
