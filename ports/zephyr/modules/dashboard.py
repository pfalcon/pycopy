try:
    import usocket as socket
except:
    import socket
import utime
import urandom

from R import R


def resp_header(resp, content_type):
        resp.write("HTTP/1.0 200 OK\r\n")
        resp.write("Content-Type: %s\r\n" % content_type)
        resp.write("\r\n")


def handle(resp, path):
    if path == b"/":
        resp_header(resp, "text/html")
        resp.write(R["static/dashboard.html"])

    elif path.startswith(b"/static/"):
        if path.endswith(b".js"):
            resp_header(resp, "text/html")
        else:
            resp_header(resp, "application/javascript")
        resp.write(R[str(path[1:], "utf-8")])

    elif path == b"/events":
        resp_header(resp, "text/event-stream")

        i = 0
        try:
            while True:
                resp.write('data: {"widget": "w_value", "field": "value1", "value": "%s"}\n\n' % urandom.getrandbits(7))
                if i & 1:
                    resp.write('data: {"widget": "w_value", "field": "value2", "value": "%s"}\n\n' % urandom.getrandbits(7))
                utime.sleep(1)
                i += 1
        except OSError:
            print("Event source connection closed")
            return

    else:
        assert 0, path


def main():
    s = socket.socket()

    # Binding to all interfaces - server will be accessible to other hosts!
    ai = socket.getaddrinfo("0.0.0.0", 8080)
    print("Bind address info:", ai)
    addr = ai[0][-1]

    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(addr)
    s.listen(5)
    print("Listening, connect your browser to http://<this_host>:8080/")

    while True:
        res = s.accept()
        client_sock = res[0]
        client_addr = res[1]
        print("Client address:", client_addr)
        print("Client socket:", client_sock)

        client_stream = client_sock

        print("Request:")
        req = client_stream.readline()
        method, path, proto = req.split(None, 2)
        print(method, path, proto)
        while True:
            h = client_stream.readline()
            if h == b"" or h == b"\r\n":
                break
            print(h)

        handle(client_stream, path)

        client_stream.close()
        print()


main()
