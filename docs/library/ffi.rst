:mod:`ffi` -- foreign function interface
========================================

.. module:: ffi
   :synopsis: FFI (foreign function interface)

This module allows to call function written in C and other languages
(from libraries, binaries, or just memory) from MicroPython programs.

Examples::

    import ffi

    libc = ffi.open("libc.so.6")
    time = libc.func("i", "time", "p")
    print("UNIX time is:", time(None))

    # Callback example

    # Use uctypes module to access data in memory
    import uctypes

    qsort = libc.func("v", "qsort", "piiC")

    def cmp(pa, pb):
        a = uctypes.bytearray_at(pa, 1)
        b = uctypes.bytearray_at(pb, 1)
        return a[0] - b[0]

    cmp_cb = ffi.callback("i", cmp, "PP")
    buf = bytearray(b"foobar")
    qsort(s, len(s), 1, cmp_cb)
    print("qsort'ed data:", buf)


Functions
---------

.. function:: open(lib)

    Open a dynamically loadable module (also known as shared library)
    and return `ffimod` object. If *lib* is ``None``, MicroPython
    executable itself if opened, which makes sense only if it was
    built to export its symbols (``--export-dynamic`` passed to linker
    when linking in it).

.. function:: func(rettype, addr, argtypes)

    Create a foreign function object for function at address *addr* (integer),
    which returns value of type *rettype* and accepts arguments of types
    *argtypes*.

.. function:: callback(rettype, func, argtypes)

    Wrap a Python function *func* in a callback object, which can be called
    from C function. *func* will be passed arguments of type *argtypes* and
    will return value of type *rettype* (currently only integer is supported).

.. class:: ffimod

    Type of object returned by `open()` function.

    .. method:: func(rettype, symname, argtypes)

        Create a foreign function object for symbol *symname* from dynamic
        module. This method is similar to the top-level `func()` function
        otherwise.

    .. method:: var(type, symname)

        Create a foreign variable object with simple scalar type
        *type* for symbol *symname* from the module. Variable value
        can be accessed using ``get()`` method on this object, and set
        using ``set()`` method.

        Note that this method makes sense only for simple variables like
        integers, floats, or pointers. For more complex types (structures,
        etc.) you should get symbol address using `addr()` method and
        handle access to it using :mod:`uctypes` module.

    .. method:: addr(symname)

        Get address of symbol *symname*. This can be passed as an argument
        to other FFI functions, or used by other modules like :mod:`uctypes`.

    .. method:: close()

        Close the `ffimod` object and unload the underlying module.

Encoding and passing values of types
------------------------------------

To specify return, parameter, and variable types, this module uses
one-letter codes, similar to those used by the :mod:`ustruct` module,
but with some changes. Please refer there for the definition of most
codes, below only differences are described:

* ``v`` - void, suitable only as a function return type.
* ``p`` - void*, pointer to writable memory. As return type, will produce
  an integer (address). As an argument, NULL pointer can be represented
  by ``None``.
* ``P`` - const void*, pointer to read-only memory. As return type, will
  produce an integer (address).
* ``s`` - As argument type, the same as ``P``. When used as return type,
  it's assumed to be a pointer to zero-terminated string, copy of that
  string is made and return as Python `str` object. NULL pointer is returned
  as ``None``.
* ``S`` - As above, but `bytes` object returned instead.
* ``O`` - Arbitrary Python object. Passed by internal representation
  (which is opaque integer/pointer) as is. This is mostly useful as
  a "callback data" parameter for functions taking callbacks. When
  used as a return type, function's value will be interpreted as a
  reference to Python object (which will lead to crash unless that's
  true.)
* ``C`` - Suitable only as argument type. A callback function as
  created by `callback()` helper.
* ``bBhHiIlLqQfd`` - Corresponding types from the :mod:`ustruct` module.

For arguments of types ``p`` and ``P``, one can pass either an integer
(representing an address), or an object implementing `buffer` protocol
(like `bytes` or `bytearray`), in which case the address of buffer's
contents will be used.
