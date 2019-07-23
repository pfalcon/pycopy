# test "errors" parameter to decoding functions

try:
    str(b'ab\xa1', 'utf-8')
except UnicodeError:
    print('UnicodeError')

try:
    b'ab\xa1'.decode('utf-8')
except UnicodeError:
    print('UnicodeError')

# CPython skips wrong data, but we don't, so check just that no exception is
# raised
str(b'ab\xa1', 'utf-8', 'ignore')
b'ab\xa1'.decode('utf-8', 'ignore')

# 'surrogateescape' isn't really implemented, any value except 'strict'
# is treated like 'ignore' (which just skips any input checks). But
# 'surrogateescape' is oftentimes used (in CPython stdlib) to roundtrip
# arbitrary bytes to str, so check that it "works".
s = str(b'ab\xa1cd', 'utf-8', 'surrogateescape')
b = bytes(s, 'utf-8', 'surrogateescape')
print(b)
s = b'ab\xa1cd'.decode('utf-8', 'surrogateescape')
b = s.encode('utf-8', 'surrogateescape')
print(b)
