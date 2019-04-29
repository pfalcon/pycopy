# test regex feature not supported by ure

try:
    import ure as re
except ImportError:
    try:
        import re
    except ImportError:
        print('SKIP')
        raise SystemExit

def test_re(r):
    try:
        re.compile(r)
        print("OK")
    except: # uPy and CPy use different errors, so just ignore the type
        print("Error")

test_re(r'a{1}')
test_re(r'a{1,2}')
test_re(r'a{1,}')
test_re(r'\a')
test_re(r'\A')
test_re(r'\b')
test_re(r'\B')
test_re(r'\f')
test_re(r'\n')
test_re(r'\r')
test_re(r'\t')
test_re(r'\u0001')
test_re(r'\U00000001')
test_re(r'\v')
test_re(r'\Z')
test_re(r'[\r\n]')
test_re(r'[a\-b]')
test_re(r'(?P<foo>bar)')
test_re(r'(?i:foo)')
test_re(r'(?#comment)')
