:mod:`ussl` -- SSL/TLS module
=============================

.. module:: ussl
   :synopsis: TLS/SSL wrapper for socket objects

|see_cpython_module| :mod:`python:ssl`.

This module provides access to Transport Layer Security (previously and
widely known as “Secure Sockets Layer”) encryption and peer authentication
facilities for network sockets, both client-side and server-side.

Functions
---------

.. function:: ussl.wrap_socket(sock, server_side=False, key=None, cert=None, cert_reqs=CERT_NONE, ca_certs=None, server_hostname=None, do_handshake=True)

   Takes a `stream` *sock* (usually usocket.socket instance of ``SOCK_STREAM`` type),
   and returns an instance of ssl.SSLSocket, which wraps the underlying stream in
   an SSL context. Returned object has the usual `stream` interface methods like
   ``read()``, ``write()``, etc. In MicroPython, the returned object does not expose
   socket interface and methods like ``recv()``, ``send()``. In particular, a
   server-side SSL socket should be created from a normal socket returned from
   :meth:`~usocket.socket.accept()` on a non-SSL listening server socket.

   Parameters:

   * *server_side* - ``False`` for client TLS socket, ``True`` for server socket.
   * *key* - TLS key in binary form. The key is mandatory for server and optional
     for client. Difference to CPython: uses *keyfile* parameter instead. In
     MicroPython, you need to pass the key content directly.
   * *cert* - TLS certificate in binary form. The certificate is mandatory for
     server and optional for client. Difference to CPython: uses *certfile* parameter
     instead.
   * *cert_reqs* - Specifies if certificates should be validated: The default is
     * ``CERT_NONE`` - default, ignore certificates, which is **INSECURE** choice.
     * ``CERT_REQUIRED`` - validate certificates. Note that validating certificates
     requires extensive resources: memory, CA certificates (*ca_certs*), secure
     real-time clock. Certificate may also be revoked, which should be checked
     separately. Validation without real-time clock, without up-to-date CA cert
     list (some CAs may be compromised), without revocation check may be **insecure**.
     Some implementations don't support this setting.
   * *ca_certs* - Specifies certificates for top-level Certificate Authorities (CAs)
     used to validate other certificates. Format of this parameter depends on the
     implementation.
   * *server_hostname* - For client sockets, hostname of the server being connected
     to, to allow virtual TLS hosts. Known as Server Name Indication (SNI). Difference
     to CPython: In CPython, this parameter does not exist for the module-level
     ``ssl.wrap_socket()`` function, but exists for SSLContext.wrap_socket()``
     method.
   * *do_handshake* - Whether to perform TLS handshake automatically, default is
     ``True``. Setting this to ``False`` is required for truly non-blocking TLS
     handling.  Difference to CPython: In CPython, this parameter does not exist
     for the module-level ``ssl.wrap_socket()`` function, but exists for
     SSLContext.wrap_socket()`` method, but named too verbosely
     *do_handshake_on_connect*.

   Depending on the underlying module implementation in a particular
   `MicroPython port`, some or all keyword arguments above may be not supported.

.. warning::

   Some implementations of ``ussl`` module do NOT validate server certificates,
   which makes an SSL connection established prone to man-in-the-middle attacks.

Exceptions
----------

.. data:: ssl.SSLError

   This exception does NOT exist. Instead its base class, OSError, is used.

Constants
---------

.. data:: ussl.CERT_NONE
          ussl.CERT_OPTIONAL
          ussl.CERT_REQUIRED

    Supported values for *cert_reqs* parameter.
