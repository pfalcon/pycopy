:mod:`uselect` -- wait for events on a set of streams
========================================================================

.. module:: uselect
   :synopsis: wait for events on a set of streams

|see_cpython_module| :mod:`python:select`.

This module provides functions to efficiently wait for events on multiple
`streams <stream>` (select streams which are ready for operations).

Functions
---------

.. function:: poll()

   Create an instance of the Poll class.

.. function:: select(rlist, wlist, xlist[, timeout])

   Wait for activity on a set of objects.

   This function is provided by some MicroPython ports for compatibility
   and is not efficient. Usage of :class:`Poll` is recommended instead.

.. _class: Poll

class ``Poll``
--------------

Methods
~~~~~~~

.. method:: poll.register(obj[, eventmask])

   Register `stream` *obj* for polling. *eventmask* is logical OR of:

   * ``uselect.POLLIN``  - data available for reading
   * ``uselect.POLLOUT`` - more data can be written

   Note that flags like ``uselect.POLLHUP`` and ``uselect.POLLERR`` are
   *not* valid as input eventmask (these are unsolicited events which
   will be returned from `poll()` regardless of whether they are asked
   for). This semantics is per POSIX.

   *eventmask* defaults to ``uselect.POLLIN | uselect.POLLOUT``.

   It is OK to call this function multiple times for the same *obj*.
   Successive calls will update *obj*'s eventmask to the value of
   *eventmask* (i.e. will behave as `modify()`).

.. method:: poll.unregister(obj)

   Unregister *obj* from polling.

.. method:: poll.modify(obj, eventmask)

   Modify the *eventmask* for *obj*. If *obj* is not registered, `OSError`
   is raised with error of ENOENT.

.. method:: poll.poll(timeout=-1)

   Wait for at least one of the registered objects to become ready or have an
   exceptional condition, with optional timeout in milliseconds (if *timeout*
   arg is not specified or -1, there is no timeout).

   Returns list of (``obj``, ``event``, ...) tuples. There may be other elements in
   tuple, depending on a platform and version, so don't assume that its size is 2.
   The ``event`` element specifies which events happened with a stream and
   is a combination of ``uselect.POLL*`` constants described above. Note that
   flags ``uselect.POLLHUP`` and ``uselect.POLLERR`` can be returned at any time
   (even if were not asked for), and must be acted on accordingly (the
   corresponding stream unregistered from poll and likely closed), because
   otherwise all further invocations of `poll()` may return immediately with
   these flags set for this stream again.

   In case of timeout, an empty list is returned.

   .. admonition:: Difference to CPython
      :class: attention

      Tuples returned may contain more than 2 elements as described above.

.. method:: poll.ipoll(timeout=-1, flags=0)

   Like :meth:`poll.poll`, but instead returns an iterator which yields a
   `callee-owned tuple`. This function provides an efficient, allocation-free
   way to poll on streams.

   If *flags* is 1, one-shot behavior for events is employed: streams for
   which events happened will have their event masks automatically reset
   (equivalent to ``poll.modify(obj, 0)``), so new events for such a stream
   won't be processed until new mask is set with `poll.modify()`. This
   behavior is useful for asynchronous I/O schedulers.

   .. admonition:: Difference to CPython
      :class: attention

      This function is a MicroPython extension.

Polling stream wrapper objects
------------------------------

MicroPython supports a concept of "stream wrapper objects", where an
original stream object (like a file or socket) is wrapped with an object
which provides stream API, but also some additional functionality. Examples
include :mod:`ussl` objects, :mod:`websocket` objects, etc. Some
MicroPython ports may allow to pass such objects to `poll.register`.
However the overall API contract for them is slightly relaxed: if such
a wrapper was returned as suitable forreading (``uselect.POLLIN``),
reading it may still lead to the EAGAIN underlying error (and e.g.
``None`` returned from .read() method). Similarly for ``uselect.POLLOUT``.
That's unlike native stream objects, for which it's guaranteed that
after ``uselect.POLLIN`` is signalled, the ``.read()`` call will return
some data (but that can be as small as 1 byte). This happens because
a wrapper object may buffer some input data and/or process it internally
(e.g. part of TLS/websocket framing and not user data transferred via
them). Applications which may accept both native and wrapper streams
should be prepared to deal with that.

Applications which are interested in the highest performance and larger
portability may instead separate concepts of "polled stream" and "I/O
stream". A polled stream is always the original stream object, before
any wrappers applied to it. An I/O stream is a top-level wrapper. An
application would keep a pair of polled and I/O streams, and use the
former to pass to ``uselect`` functions, while the latter - to read/write
(still being ready to receive EAGAIN/None as described above).
