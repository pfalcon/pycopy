Builtin functions and exceptions
================================

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

      In MicroPython, `byteorder` parameter must be positional (this is
      compatible with CPython).

   .. method:: to_bytes(size, byteorder)

      In MicroPython, `byteorder` parameter must be positional (this is
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

   .. admonition:: MicroPython extension
      :class: note

      A `str` object can be passed to memoryview() constructor, in which
      case underlying UTF-8 encoded data can be accessed by memoryview.
      ``memoryview("abc")`` is equivalent to ``memoryview("abc".encode())``,
      but is more memory-efficient.

   .. admonition:: MicroPython extension
      :class: note

      Additional *offset* and *size* parameters can be passed to constructor.
      ``mv = memoryview(buf, offset, size)`` is equivalent to
      ``mv1 = memoryview(buf); mv = mv1[offset:offset + size]``, but is more
      memory-efficient.

   .. method:: init(buffer[, offset, size])

      Reinitialize existing `memoryview` object inplace with a new buffer,
      and optionally given *offset* and *size* into it.

      .. admonition:: MicroPython extension
         :class: note

         This method is a MicroPython extension. It doesn't require memory
         allocation. Just as the constructor, this method can be called
         for `str` objects too.

   Example of usage of MicroPython extensions::

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
            # We use another MicroPython extension, 3-argument stream
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

    |see_cpython| `python:OSError`. MicroPython doesn't implement ``errno``
    attribute, instead use the standard way to access exception arguments:
    ``exc.args[0]``.

.. exception:: RuntimeError

.. exception:: StopIteration

.. exception:: SyntaxError

.. exception:: SystemExit

    |see_cpython| `python:SystemExit`.

.. exception:: TypeError

    |see_cpython| `python:TypeError`.

.. exception:: ValueError

.. exception:: ZeroDivisionError
