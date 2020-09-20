.. _unix_quickref:

Quick reference for the Unix and Windows ports
==============================================

Command line options
--------------------

Usage::

    pycopy [ -i ] [ -O<level> ] [ -v ] [ -X <option> ] [ -c <command> | -m <module> | <script> ] [ <args> ]
    pycopy-dev [ -i ] [ -O<level> ] [ -v ] [ -X <option> ] [ -c <command> | -m <module> | <script> ] [ <args> ]

``pycopy`` executable provides native Pycopy environment and API. It is
recommended for developing native Pycopy software, which can run on the
wide range of target platforms (including low-end). ``pycopy-dev`` is
a version with various additional features and extensibility enabled,
offering better CPython compatibility at the expense of being larger
and slower.

Invocation options:

.. option:: -c <command>

   Runs the code in ``<command>``. The code can be one or more Python statements.

.. option:: -m <module>

   Runs the module ``<module>``. The module must be in ``sys.path``.

.. option:: <script>

   Runs the file ``<script>``.

If none of the 3 options above are given, then interpreter is run in an
interactive REPL mode.


.. option:: <args>

    Any additional arguments after the module or script will be passed to
    ``sys.argv`` (not supported with the :option:`-c` option).


General options:

.. option:: -i

    Enables inspection. When this flag is set, interpreter will enter the
    interactive REPL mode after the command, module or script has finished.
    This can be useful for debugging the state after an unhandled exception.

.. option:: -O | -O<level> | -OO...

    Sets the optimization level. The ``O`` can be followed by a number or can
    be repeated multiple times to indicate the level. E.g. ``-O3`` is the same
    as ``-OOO``.

.. option:: -v

    Increases the verbosity level. This option can be given multiple times.
    This option only has an effect if ``MICROPY_DEBUG_PRINTERS`` was enabled
    when Pycopy itself was compiled.

.. option:: -X <option>

    Specifies additional implementation-specific options. Possible options are:

    - ``-X compile-only`` compiles the command, module or script but does not
      run it.
    - ``-X emit={bytecode,native,viper}`` sets the default code emitter. Native
      emitters may not be available depending on the settings when Pycopy
      itself was compiled.
    - ``-X heapsize=<n>[w][K|M]`` sets the heap size, i.e. maximum amount of memory
      a Python application can use, in bytes. Suffixes ``K`` and ``M`` mean kilobytes
      and megabytes respectively. If after the number, but before ``K`` or ``M``
      there's a suffix ``w``, it means adjust the specified size for word size of
      the machine, taking 32-bit word size as the base. E.g., ``-X heapsize=10wK``
      will create a heap which may contain about the same number of objects on
      both 32-bit and 64-bit machines (the actual size will be 10K on 32-bit machines
      and 20K on 64-bit machines).



Environment variables
---------------------

.. envvar:: PYCOPYPATH

    Overrides the default search path for Pycopy libraries. ``PYCOPYPATH``
    should be set to a colon separated list of directories. If ``PYCOPYPATH`` is
    not defined, the search path will be ``~/.pycopy/lib:/usr/lib/pycopy``
    or the value of the ``MICROPY_PY_SYS_PATH_DEFAULT`` option if it was set
    when Pycopy itself was compiled.
