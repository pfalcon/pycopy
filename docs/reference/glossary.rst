Glossary
========

.. glossary::

    baremetal
        A system without a (full-fledged) OS, for example an
        :term:`MCU`-based system. When running on a baremetal system,
        MicroPython effectively becomes its user-facing OS with a command
        interpreter (REPL).

    board
        A PCB board. Oftentimes, the term is used to denote a particular
        model of an :term:`MCU` system. Sometimes, it is used to actually
        refer to :term:`MicroPython port` to a particular board (and then
        may also refer to "boardless" ports like
        :term:`Unix port <MicroPython Unix port>`).

    buffer
        An object implementing a buffer protocol. The simplest examples of
        buffers are `bytes` (read-only buffer) and `bytearray` (read-write
        buffer).

    callee-owned tuple
        A tuple returned by some builtin function/method, containing data
        which is valid for a limited time, usually until next call to the
        same function (or a group of related functions). After next call,
        data in the tuple may be changed. This leads to the following
        restriction on the usage of callee-owned tuples - references to
        them cannot be stored. The only valid operation is extracting
        values from them (including making a copy). Callee-owned tuples
        is a MicroPython-specific construct (not available in the general
        Python language), introduced for memory allocation optimization.
        The idea is that callee-owned tuple is allocated once and stored
        on the callee side. Subsequent calls don't require allocation,
        allowing to return multiple values when allocation is not possible
        (e.g. in interrupt context) or not desirable (because allocation
        inherently leads to memory fragmentation). Note that callee-owned
        tuples are effectively mutable tuples, making an exception to
        Python's rule that tuples are immutable. (It may be interesting
        why tuples were used for such a purpose then, instead of mutable
        lists - the reason for that is that lists are mutable from user
        application side too, so a user could do things to a callee-owned
        list which the callee doesn't expect and could lead to problems;
        a tuple is protected from this.) There is another way to summarize
        the situation: a callee-owned tuple is a special subtype of the
        normal `tuple` type, with the semantics described above, namely:
        a user application cannot change its contents, but some special
        functions can. In the current implementation, this special subtype
        actually coincides with the main tuple type for efficiency reasons
        (and this is an implementation detail with which users should not
        be concerned).

    CPython
        CPython is the reference implementation of Python programming
        language, and the most well-known one, which most of the people
        run. It is however one of many implementations (among which
        Jython, IronPython, PyPy, and many more, including MicroPython).
        As there is no formal specification of the Python language, only
        CPython documentation, it is not always easy to draw a line
        between Python the language and CPython its particular
        implementation. This however leaves more freedom for other
        implementations. For example, MicroPython does a lot of things
        differently than CPython, while still aspiring to be a Python
        language implementation.

    fragmentation
        Fragmentation is a phenomenon of any dynamic memory allocation
        system when free memory is spread across a number of chunks,
        so while total amount of free memory may be high, allocating
        a contiguous chunk of memory is possible only within the size
        of the largest free chunk. Note that fragmentation is not just
        an issue of scripting languages or MicroPython in particular,
        a C program performing dynamic allocation is subject to the
        same issues. To understand a problem better, consider following
        example: there're 4K of memory total, and application consecutively
        allocates 1K chunks, numbered #0, #1, #2, #3. Then if application
        frees chunks #1 and #3, there're total 2K of free memory. However,
        it's possible to allocate a chunk with the maximum size of 1K only.
        Measures to fight fragmentation include: 1) preallocation (static
        allocation) and inplace operations; 2) compacting garbage
        collection. Static allocation is a well-known technique oftentimes
        used in C. Unlike many other scripting languages, Python provides
        excelent capabilities for inplace operations on preallocated
        buffers and objects, and MicroPython extends it even further.
        Compacting garbage collection is however an open topic.

    GPIO
        General-purpose input/output. The simplest means to control
        electrical signals. With GPIO, user can configure hardware
        signal pin to be either input or output, and set or get
        its digital signal value (logical "0" or "1"). MicroPython
        abstracts GPIO access using :class:`machine.Pin` and :class:`machine.Signal`
        classes.

    GPIO port
        A group of :term:`GPIO` pins, usually based on hardware
        properties of these pins (e.g. controllable by the same
        register).

    interned string
        A string referenced by its (unique) identity rather than its
        address. Interned strings are thus can be quickly compared just
        by their identifiers, instead of comparing by content. The
        drawbacks of interned strings are that interning operation takes
        time (proportional to the number of existing interned strings,
        i.e. becoming slower and slower over time) and that the space
        used for interned strings is not reclaimable. String interning
        is done automatically by MicroPython compiler and runtime when
        it's either required by the implementation (e.g. function keyword
        arguments are represented by interned string id's) or deemed
        beneficial (e.g. for short enough strings, which have a chance
        to be repeated, and thus interning them would save memory on
        copies). Most of string and I/O operations don't produce interned
        strings due to drawbacks described above.

    MCU
        Microcontroller. Microcontrollers usually have much less resources
        than a full-fledged computing system, but smaller, cheaper and
        require much less power. MicroPython is designed to be small and
        optimized enough to run on an average modern microcontroller.

    memory allocation
        Computers store data in memory, and memory allocation is a process
        they perform to store *new* data in memory. This process has its
        cost (in terms of time required), as usually involves scanning
        thru memory to find a suitable free chunk. It may also fail if
        suitable free chunk is not found. Computers also have "registers",
        which allow to store limited amount of data without special memory
        allocation. While MicroPython is a high-level language, these basic
        traits of computers still apply to some aspects of its functioning,
        and worth to keep in mind when e.g. optimizing an application, or
        trying to achieve real-time/failure-free operation.

        MicroPython stores majority of objects in memory, thus when creating
        a new object it needs to perform memory allocation. However, there
        are exceptions. Some special objects may be created without
        allocation. One notable example is :term:`small integer`'s. There may
        be also other objects like, e.g. short repeated strings which are
        automatically :term:`interned <interned string>`, etc. These are
        however considered an implementation detail, and often differ
        by a :term:`MicroPython port`.

        Besides using allocation-free objects (set of which is very limited,
        as explained above), there's another way to avoid, or at least limit
        memory allocation: avoid creating new objects during operations (and
        growing object size, as that leads to the need to allocate more memory
        too). These are known as inplace operations.

        An advanced MicroPython programmer should know about the memory
        allocation aspects because:

        * MicroPython features automatic memory management. Allocation
          operations are usually performed fast, until available memory
          is exhausted, then garbage collection (GC) needs to be performed.
          The GC is a relatively long operation, which can lead to delays
          in application response.
        * Allocation leads to :term:`fragmentation`.
        * If GC didn't reclaim free block of memory of suitable size (which
          can be due to :term:`fragmentation`), allocation will simply fail,
          aborting an application unless special care is taken.
        * Even without effects of GC, memory allocation takes non-zero
          time, and this time may vary. This may both slow down tight
          processing loops, and make them non real-time (processing time
          may vary noticeably).
        * Memory allocation may be disallowed in special execution contexts,
          e.g. in interrupt handlers.

    micropython-lib
        MicroPython is (usually) distributed as a single executable/binary
        file with just few builtin modules. There is no extensive standard
        library comparable with :term:`CPython`. Instead, there is a related, but
        separate project
        `micropython-lib <https://github.com/micropython/micropython-lib>`_
        which provides implementations for many modules from CPython's
        standard library. However, large subset of these modules require
        POSIX-like environment (Linux, FreeBSD, MacOS, etc.; Windows may be
        partially supported), and thus would work or make sense only with
        `MicroPython Unix port`. Some subset of modules is however usable
        for `baremetal` ports too.

        Unlike monolithic :term:`CPython` stdlib, micropython-lib modules
        are intended to be installed individually - either using manual
        copying or using :term:`upip`.

    MicroPython port
        MicroPython supports different :term:`boards <board>`, RTOSes,
        and OSes, and can be relatively easily adapted to new systems.
        MicroPython with support for a particular system is called a
        "port" to that system. Different ports may have widely different
        functionality. This documentation is intended to be a reference
        of the generic APIs available across different ports ("MicroPython
        core"). Note that some ports may still omit some APIs described
        here (e.g. due to resource constraints). Any such differences,
        and port-specific extensions beyond MicroPython core functionality,
        would be described in the separate port-specific documentation.

    MicroPython Unix port
        Unix port is one of the major :term:`MicroPython ports <MicroPython port>`.
        It is intended to run on POSIX-compatible operating systems, like
        Linux, MacOS, FreeBSD, Solaris, etc. It also serves as the basis
        of Windows port. The importance of Unix port lies in the fact
        that while there are many different :term:`boards <board>`, so
        two random users unlikely have the same board, almost all modern
        OSes have some level of POSIX compatibility, so Unix port serves
        as a kind of "common ground" to which any user can have access.
        So, Unix port is used for initial prototyping, different kinds
        of testing, development of machine-independent features, etc.
        All users of MicroPython, even those which are interested only
        in running MicroPython on :term:`MCU` systems, are recommended
        to be familiar with Unix (or Windows) port, as it is important
        productivity helper and a part of normal MicroPython workflow.

    port
        Either :term:`MicroPython port` or :term:`GPIO port`. If not clear
        from context, it's recommended to use full specification like one
        of the above.

    small integer
        An integer value of limited range which can be produced and operated
        on without memory allocation. See :term:`memory allocation` for why this
        is useful. A small integer fits within a machine word, and as it
        needs to be distinguished from values of other types, which is done
        by means of a special tag bit(s) in a machine word, it has necessarily
        small range than the machine word. To reinstate that, a small int cannot
        hold an entire value of a machine word, which is useful fact to keep in
        mind for developers interested in optimization, e.g. for real-time
        operations. Also keep in mind that Python integers are signed, so
        small integer is signed too. As an example, with minimum 1 bit required
        for a tag, and 1 bit for a sign, on a typical 32-bit system, a small
        integer can hold a value in range ``-2**30 .. 2**30-1``, or roughly
        +/- one billion.

    stream
        Also known as a "file-like object". An object which provides sequential
        read-write access to the underlying data. A stream object implements
        a corresponding interface, which consists of methods like ``read()``,
        ``write()``, ``readinto()``, ``seek()``, ``flush()``, ``close()``, etc.
        A stream is an important concept in MicroPython, many I/O objects
        implement the stream interface, and thus can be used consistently and
        interchangeably in different contexts. For more information on
        streams in MicroPython, see `uio` module.

    upip
        (Literally, "micro pip"). A package manage for MicroPython, inspired
        by :term:`CPython`'s pip, but much smaller and with reduced functionality.
        upip runs both on :term:`Unix port <MicroPython Unix port>` and on
        :term:`baremetal` ports (those which offer filesystem and networking
        support).
