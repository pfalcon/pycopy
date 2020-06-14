try:
    import usocket as _socket
except:
    import _socket
try:
    import ussl as ssl
except:
    import ssl

if hasattr(ssl, "SSLContext"):
    # CPython and Pycopy support SSLContext, and Python API supports
    # server_hostname only on SSLContext.
    ctx = ssl.SSLContext()
else:
    # Original MicroPython API doesn't have SSLContext, but supports
    # server_hostname param to module-global wrap_socket().
    ctx = ssl


def test_one(site, opts):
    ai = _socket.getaddrinfo(site, 443)
    addr = ai[0][-1]

    s = _socket.socket()

    try:
        s.connect(addr)

        if "sni" in opts:
            s = ctx.wrap_socket(s, server_hostname=opts["host"])
        else:
            s = ctx.wrap_socket(s)

        s.write(b"GET / HTTP/1.0\r\nHost: %s\r\n\r\n" % bytes(site, "latin"))
        resp = s.read(4096)
        # print(resp)

    finally:
        s.close()


SITES = [
    "google.com",
    "www.google.com",
    "api.telegram.org",
    {"host": "api.pushbullet.com", "sni": True},
    # From https://docs.aws.amazon.com/iot/latest/developerguide/http.html
    {"host": "a3qj468xinsffp-ats.iot.us-west-2.amazonaws.com", "sni": True},
]


def main():
    for site in SITES:
        opts = {}
        if isinstance(site, dict):
            opts = site
            site = opts["host"]

        try:
            test_one(site, opts)
            print(site, "ok")
        except Exception as e:
            print(site, repr(e))


main()
