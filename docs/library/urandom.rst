:mod:`urandom` -- pseudo-random number generation
=================================================

.. module:: urandom
   :synopsis: pseudo-random number generation

|see_cpython_module| :mod:`python:random`.

This module is used to generate **pseudo-random** numbers. On all Pycopy
platforms, this module is **not seeded** on startup, which means that if
you do not explicitly call `seed()` function with a suitable varying value,
the numbers returned by the `getrandbits()` function will be the same for
each fresh interpreter session. This is a deliberate design decision, to
emphasize the fact that this module does **not** provide true randomness.

Seeding the pseudo-random generator lies on a user application,
by whatever means suitable for a particular application. For example,
having a pseudo-random, but stable predictable sequence (as provided by
this module by default) is useful for reproducible testing. For some
applications, seeding with the current time (perhaps, lower milliseconds
or microseconds of it, as provided by `utime.ticks_ms()` or
`utime.ticks_us()`) would be enough. Yet for other, a true random
generator may be required. (Note that this module does not guarantee
that it is a cryptographically secure random number generator). For
systems which support it, a high-quality random number generator should
be provided by `uos.urandom()` function (values from which can be used
to seed this module).

Functions
---------

.. function:: seed(value)

    Seed pseudo-random generator with an integer *value*, which should
    be sufficiently random itself. As explained above, if this function
    is not called, the numbers returned from `getrandbits()` will be
    the same for each session.

.. function:: getrandbits(num_bits)

    Return an unsigned (non-negative) pseudo-random number of *num_bits*
    bit size, i.e. in the range of [0, 2**num_bits - 1] (inclusive).
    Miximum supported value of *num_bits* is 32, though some implementations
    may apply even lower limit (so use the value not higher than actually
    needed for a particular use).
