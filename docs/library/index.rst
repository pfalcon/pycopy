.. _micropython_lib:

Pycopy libraries
================

.. warning::

   Important summary of this section:

   * Pycopy implements a subset of Python functionality for each module.
   * To ease extensibility, Pycopy versions of standard Python modules
     usually have ``u`` ("micro") prefix, e.g. ``utime`` instead of ``time``.
   * Any particular `Pycopy port` or variant may miss any feature/function
     described in the general documentation (due to resource constraints or
     other limitations).
   * More complete and compatible with CPython modules are available via
     the `pycopy-lib` project.


This chapter describes modules (function and class libraries) which are built
into Pycopy. There are a few categories of such modules:

* Modules which implement a subset of standard Python functionality and are not
  intended to be extended by the user.
* Modules which implement a subset of Python functionality, with a provision
  for extension by the user (via Python code).
* Modules which implement Pycopy extensions to the Python standard libraries.
* Modules specific to a particular `Pycopy port` and thus not portable to
  other ports.

Note about the availability of the modules and their contents: This documentation
in general aspires to describe all modules and functions/classes which are
implemented in Pycopy project. However, Pycopy is highly configurable, and
each port to a particular board/embedded system makes available only a subset
of Pycopy libraries. For officially supported ports, there is an effort
to either filter out non-applicable items, or mark individual descriptions
with "Availability:" clauses describing which ports provide a given feature.

With that in mind, please still be warned that some functions/classes
in a module (or even the entire module) described in this documentation **may be
unavailable** in a particular build of Pycopy on a particular system. The
best place to find general information of the availability/non-availability
of a particular feature is the "General Information" section which contains
information pertaining to a specific `Pycopy port`.

On some ports you are able to discover the available, built-in libraries that
can be imported by entering the following at the REPL::

    help('modules')

Beyond the built-in libraries described in this documentation, many more
modules from the Python standard library, as well as further Pycopy
extensions to it, can be found in `pycopy-lib`.

Python standard libraries and micro-libraries
---------------------------------------------

The following standard Python libraries have been "micro-ified" to fit in with
the philosophy of Pycopy.  They provide the core functionality of that
module and are intended to be a drop-in replacement for the standard Python
library.  Some modules below use a standard Python name, but prefixed with "u",
e.g. ``ujson`` instead of ``json``. This is to signify that such a module is
micro-library, i.e. implements only a subset of CPython module functionality.
By naming them differently, a user has a choice to write a Python-level module
to extend functionality for better compatibility with CPython (indeed, this is
what done by the `pycopy-lib` project mentioned above).

On some embedded platforms, where it may be cumbersome to add Python-level
wrapper modules to achieve naming compatibility with CPython, micro-modules
are available both by their u-name, and also by their non-u-name.  The
non-u-name can be overridden by a file of that name in your library path (``sys.path``).
For example, ``import json`` will first search for a file ``json.py`` (or package
directory ``json``) and load that module if it is found.  If nothing is found,
it will fallback to loading the built-in ``ujson`` module. This behavior is
deprecated and not compatible with other platforms on which Pycopy runs.
Please do not rely on it.

.. toctree::
   :maxdepth: 1

   builtins.rst
   cmath.rst
   gc.rst
   math.rst
   sys.rst
   uarray.rst
   ubinascii.rst
   ucollections.rst
   uerrno.rst
   uhashlib.rst
   uheapq.rst
   uio.rst
   ujson.rst
   uos.rst
   urandom.rst
   ure.rst
   uselect.rst
   usocket.rst
   ussl.rst
   ustruct.rst
   utime.rst
   uzlib.rst
   _thread.rst


Pycopy-specific libraries
-------------------------

Functionality specific to the Pycopy implementation is available in
the following libraries.

.. toctree::
   :maxdepth: 1

   btree.rst
   framebuf.rst
   machine.rst
   micropython.rst
   network.rst
   ubluetooth.rst
   ucryptolib.rst
   uctypes.rst


Port-specific libraries
-----------------------

In some cases the following port/board-specific libraries have functions or
classes similar to those in the :mod:`machine` library.  Where this occurs, the
entry in the port specific library exposes hardware functionality unique to
that platform.

To write portable code use functions and classes from the :mod:`machine` module.
To access platform-specific hardware use the appropriate library, e.g.
:mod:`pyb` in the case of the Pyboard.

Libraries specific to the Unix port
-----------------------------------

The following libraries are specific to the `Pycopy Unix port`.

.. toctree::
  :maxdepth: 1

  ffi.rst

Libraries specific to the pyboard
---------------------------------

The following libraries are specific to the pyboard.

.. toctree::
  :maxdepth: 1

  pyb.rst
  lcd160cr.rst


Libraries specific to the WiPy
------------------------------

The following libraries and classes are specific to the WiPy.

.. toctree::
  :maxdepth: 1

  wipy.rst
  machine.ADCWiPy.rst
  machine.TimerWiPy.rst


Libraries specific to the ESP8266 and ESP32
-------------------------------------------

The following libraries are specific to the ESP8266 and ESP32.

.. toctree::
  :maxdepth: 1

  esp.rst
  esp32.rst
