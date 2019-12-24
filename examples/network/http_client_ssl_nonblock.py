# Example of working with non-blocking "wrapper" sockers, SSL sockets in this
# case. Working with complex wrapper sockets involves handling of special
# uio.WANT_READ return from .write(), and uio.WANT_WRITE from .read(). This
# is in addition to handling None special return from both of these.
# For comparison, in CPython, instead of special return values, there're
# exceptions SSLWantReadError, SSLWantWriteError, which adhoc to its "ssl"
# module (whereas Pycopy's mechanism is both efficient and fully generic,
# other wrapper streams, e.g. websockets, would work the same way).
import utime
import uio
import usocket
import ussl
import uselect


def wait_sock(s, poll_flag):
    poll = uselect.poll()
    poll.register(s, poll_flag)
    t = utime.time()
    res = poll.poll()
    print(res)
    print("Became readable in:", utime.time() - t, "s")


def write_all(io_sock, poll_sock, data):
    while data:
        res = io_sock.write(data)
        print("write_all:", res)
        if res is None:
            wait_sock(poll_sock, uselect.POLLOUT)
        elif res == uio.WANT_READ:
            wait_sock(poll_sock, uselect.POLLIN)
        elif res > 0:
            data = data[res:]
        else:
            assert 0, "Unexpected return from ussl write: %s" % res

    print("write_data done")


def read_all(io_sock, poll_sock, size):
    buf = b""
    while size:
        res = io_sock.read(size)
        print("read_all:", res)
        if res is None:
            wait_sock(poll_sock, uselect.POLLIN)
        elif res == uio.WANT_WRITE:
            wait_sock(poll_sock, uselect.POLLOUT)
        elif res == b"":
            break
        elif isinstance(res, bytes):
            buf += res
            if size != -1:
                size -= len(res)
        else:
            assert 0, "Unexpected return from ussl read: %s" % res

    print("read_data done")
    return buf


def main():
    underlying_socket = usocket.socket()

    ai = usocket.getaddrinfo("google.com", 443)
    print("Address infos:", ai)
    addr = ai[0][-1]

    print("Connect address:", addr)
    underlying_socket.connect(addr)

    wrapper_socket = ussl.wrap_socket(underlying_socket, do_handshake=False)
    wrapper_socket.setblocking(False)
    print(wrapper_socket)

    write_all(wrapper_socket, underlying_socket, b"GET / HTTP/1.0\r\n\r\n")
    print(read_all(wrapper_socket, underlying_socket, -1))

    wrapper_socket.close()
    underlying_socket.close()


main()
