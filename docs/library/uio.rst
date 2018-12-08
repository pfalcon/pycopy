:mod:`uio` -- input/output streams
==================================

.. module:: uio
   :synopsis: input/output streams

|see_cpython_module| :mod:`python:io`.

This module contains additional types of `stream` (file-like) objects
and helper functions.

Conceptual hierarchy
--------------------

.. admonition:: Difference to CPython
   :class: attention

   Conceptual hierarchy of stream base classes is simplified in MicroPython,
   as described in this section.

(Abstract) base stream classes, which serve as a foundation for behavior
of all the concrete classes, adhere to few dichotomies (pair-wise
classifications) in CPython. In MicroPython, they are somewhat simplified
and made implicit to achieve higher efficiencies and save resources.

An important dichotomy in CPython is unbuffered vs buffered streams. In
MicroPython, all streams are currently unbuffered. This is because all
modern OSes, and even many RTOSes and filesystem drivers already perform
buffering on their side. Adding another layer of buffering is counter-
productive (an issue known as "bufferbloat") and takes precious memory.
Note that there are still cases where buffering may be useful, so we may
introduce optional buffering support at a later time.

But in CPython, another important dichotomy is tied with "bufferedness" -
it's whether a stream may incur short read/writes or not. A short read
is when a user asks e.g. 10 bytes from a stream, but gets less, similarly
for writes. In CPython, unbuffered streams are automatically short
operation susceptible, while buffered are guaranteed against them. The
no short read/writes is an important trait, as it allows to develop
more concise and efficient programs - something which is highly desirable
for MicroPython. So, while MicroPython doesn't support buffered streams,
it still provides for no-short-operations streams. Whether there will
be short operations or not depends on each particular class' needs, but
developers are strongly advised to favor no-short-operations behavior
for the reasons stated above. For example, MicroPython sockets are
guaranteed to avoid short read/writes. Actually, at this time, there is
no example of a short-operations stream class in the core, and one would
be a port-specific class, where such a need is governed by hardware
peculiarities.

The no-short-operations behavior gets tricky in case of non-blocking
streams, blocking vs non-blocking behavior being another CPython dichotomy,
fully supported by MicroPython. Non-blocking streams never wait for
data either to arrive or be written - they read/write whatever possible,
or signal lack of data (or ability to write data). Clearly, this conflicts
with "no-short-operations" policy, and indeed, a case of non-blocking
buffered (and thus no-short-ops) streams is convoluted in CPython - in
some places, such combination is prohibited, in some it's undefined or
just not documented, in some cases it raises verbose exceptions. The
matter is much simpler in MicroPython: non-blocking stream are important
for efficient asynchronous operations, so this property prevails
the "no-short-ops" one. So, while blocking streams will avoid short
reads/writes whenever possible (the only case to get a short read is
if the end of file is reached, or in a case of error (but errors don't
return short data, but raise exceptions)), non-blocking streams may
produce short data to avoid blocking the operation.

The final dichotomy is binary vs text streams. MicroPython of course
supports these, but while in CPython text streams are inherently
buffered, they aren't in MicroPython. (Indeed, that's one of the cases
for which we may introduce buffering support.)

Note that for efficiency, MicroPython doesn't provide abstract base
classes corresponding to the hierarchy above, and it's not possible
to implement, or subclass, a stream class in pure Python.

Abstract stream interface
-------------------------

Warning: AbstractStream is an implied abstract class, described here
for illustrative purposes. There is no actual class with this name,
but all other stream classes implement subset of the functionality
described here.

.. class:: AbstractStream

    .. method:: read(size=-1)

        Read *size* bytes or characters from the stream, or until EOF if
        size is -1 or not specified. Return `str` or `bytes` with
        contents read. Return ``None`` if stream is in non-blocking mode
        and no data is available.

    .. method:: readline(maxsize=-1)

        Read a line (sequence of bytes or characters) from the stream
        until ``'\n'`` character is seen (or until EOF, or until no more
        data is available in non-blocking mode). The line, with ``'\n'``
        included, is returned. If positional *maxsize* argument is provided,
        at most *maxsize* bytes are read (in which case the returned
        string might not end with ``'\n'``). If stream is in non-blocking
        mode and no data available, ``None`` is returned.

    .. method:: readinto(buf, [maxsize])

        Read bytes into a *buf* `buffer`. The size of data read is equal
        to the size of buffer (unless EOF/lack of non-blocking data occurs
        first), unless *maxsize* is specified, which allows to read less
        data. Returns number of bytes actually read. This method is
        available only for binary streams.

        MicroPython extension: *buf* can be a `BytesIO` object. Data
        will be written at the current offset of the BytesIO object, and
        up to remaining allocation size of data will be written, in other
        words, this operation will not grow the internal buffer of BytesIO
        object.

    .. method:: write(data)

        Writes to the stream *data*, which should be `str` for text streams,
        or arbitrary `buffer` for binary streams. Returns number of items
        (bytes or characters) actually written, or None if non-blocking
        stream could not accept any data.

    .. method:: write(data, size)
                write(data, offset, size)

        MicroPython extension: write a substring of *data*, starting at *offset*
        (or 0), and with the given *size*. For example,
        ``write(b'12345', 1, 2)`` will write ``b'23'`` to the stream. These
        methods are useful as an optimization when working with short-write
        streams (e.g., non-blocking streams), to avoid slicing or creating
        `memoryview` object to write the remaining chunk of data.

    .. method:: flush()

        Flush any data or metadata, cached internally (in MicroPython
        components) or externally (e.g. in OS) to the underlying medium.
        For example, for files, all data will be written to disk, for
        network streams - data will be sent over network, etc.

    .. method:: close()

        Close the stream. No other operations on stream are possible after
        the closure (will lead to error or underfines behavior). However,
        the close() operation itself should be idempotent, i.e. it should
        be possible to call in multiple times without an error (2nd and
        following calls should not lead to any effect).

    .. method:: seek(offset, [whence])

        Move internal stream offset pointer for random-access streams.
        Read/write operations are performed from the position specified
        by this pointer. *whence* parameter can be:

        * 0 - *offset* is against the start of stream (known as SEEK_SET)
        * 1 - *offset* is against the current positions (known as SEEK_CUR)
        * 2 - *offset* is against the end of stream (known as SEEK_END)

        Note that the symbolic names (SEEK_SET/SEEK_CUR/SEEK_END) are **NOT**
        provided by the ``uio`` module, to minimize the code size. Instead,
        well-known values 0, 1, 2 can be used. Your application may define
        these symbolic names itself, or use ``io`` module from
        `micropython-lib` which provides them.

        This method is available only for random-access streams.

    .. method:: truncate(size=None)

        Resize the stream to the given *size*, or the current stream
        position if *size* is not provided. Note that this operation
        can both grow and shrink stream with respect to its current size.
        This method is available only for random-access streams.

    .. method:: setblocking(val)

        If *val* is ``False``, the stream is swtiched to non-blocking mode.
        Otherwise, if ``True``, it is switched to blocking mode.

    .. method:: settimeout(val)

        Set timeout for stream operations. This is generalization of
        `setblocking()` method. If *val* is 0, the stream is switched to
        non-blocking mode. if *val* is ``None``, the stream is switched
        to blocking mode. Otherwise, *val* represents a timeout in seconds
        for stream operations. If an operation is not completed in the time
        alloted, ``OSError(ETIMEDOUT)`` is raised.

Functions
---------

.. function:: open(name, mode='r', **kwargs)

    Open a file. Builtin ``open()`` function is aliased to this function.
    All ports (which provide access to file system) are required to support
    *mode* parameter, but support for other arguments vary by port.

Classes
-------

.. class:: FileIO(...)

    This is type of a file open in binary mode, e.g. using ``open(name, "rb")``.
    You should not instantiate this class directly.

.. class:: TextIOWrapper(...)

    This is type of a file open in text mode, e.g. using ``open(name, "rt")``.
    You should not instantiate this class directly.

.. class:: StringIO([string])
.. class:: BytesIO([string])

    In-memory file-like objects for input/output. `StringIO` is used for
    text-mode I/O (similar to a normal file opened with "t" modifier).
    `BytesIO` is used for binary-mode I/O (similar to a normal file
    opened with "b" modifier). Initial contents of file-like objects
    can be specified with *string* parameter (should be normal string
    for `StringIO` or bytes object for `BytesIO`). All the usual file
    methods like ``read()``, ``write()``, ``seek()``, ``flush()``,
    ``close()`` are available on these objects, and additionally, a
    following method:

    .. method:: getvalue()

        Get the current contents of the underlying buffer which holds data.

.. class:: StringIO(alloc_size)
.. class:: BytesIO(alloc_size)

    Create an empty `StringIO`/`BytesIO` object, preallocated to hold up
    to *alloc_size* number of bytes. That means that writing that amount
    of bytes won't lead to reallocation of the buffer, and thus won't hit
    out-of-memory situation or lead to memory fragmentation. These constructors
    are a MicroPython extension and are recommended for usage only in special
    cases and in system-level libraries, not for end-user applications.

    .. admonition:: Difference to CPython
        :class: attention

        These constructors are a MicroPython extension.
