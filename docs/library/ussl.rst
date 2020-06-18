:mod:`ussl` -- SSL/TLS module
=============================

.. module:: ussl
   :synopsis: TLS/SSL wrapper for socket objects

|see_cpython_module| :mod:`python:ssl`.

This module provides access to Transport Layer Security (previously and
widely known as “Secure Sockets Layer”) encryption and peer authentication
facilities for network sockets, both client-side and server-side.

Classes
-------

.. class:: SSLContext()

    Create a context object containing settings applicable across a number of
    individual TLS/SSL connections (sockets). For example, this includes
    server certificate and private key - all connections to this server will
    share this certificate and key.

    .. method:: set_cert_key(cert, key)

        Set certificate and corresponding private key to use for this context:

        * *cert* - TLS certificate in binary DER encoding. The certificate is
          mandatory for server and optional for client.
        * *key* - TLS private key in binary DER encoding. The key is mandatory
          for server and optional for client.

        Difference to CPython: CPython uses a different method and loads
        certificate/key from a file. In Pycopy, you need to pass the
        certificate/key content directly.

    .. method:: wrap_socket(sock, server_side=False, server_hostname=None, do_handshake=True)

        Takes a `stream` *sock* (usually `usocket.socket` instance of ``SOCK_STREAM`` type),
        and returns an instance of ssl.SSLSocket, which wraps the underlying stream in
        an SSL context. Returned object has the usual `stream` interface methods like
        ``read()``, ``write()``, etc. In Pycopy, the returned object does not expose
        socket interface and methods like ``recv()``, ``send()``, ``listen()``, etc.
        In particular, a server-side SSL socket should be created from a normal
        socket returned from :meth:`~usocket.socket.accept()` on a non-SSL listening
        server socket.

        Parameters:

        * *server_side* - ``False`` for client TLS socket, ``True`` for server socket.
        * *server_hostname* - For client sockets, hostname of the server being connected
          to, to allow virtual TLS hosts. This features is known as Server Name Indication
          (SNI).
        * *do_handshake* - Whether to perform TLS handshake automatically, default is
          ``True``. Setting this to ``False`` is required for truly non-blocking TLS
          handling.  Difference to CPython: In CPython, this parameter is named too
          verbosely as *do_handshake_on_connect*.

        Depending on the underlying module implementation in a particular
        :term:`Pycopy port`, some or all keyword arguments above may be not supported.

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
