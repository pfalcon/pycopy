# Ensure that specific string formatting uses predictable amount of heap
import micropython

# Based on inside knowledge that str % starts with 16 bytes output buffer ;-)
FUZZ = 16

# Use vars to guard against possible constant folding
a = "a"
b = "b"

m = None
# To allow to experiment with different mem_* funcs easily
func = micropython.mem_total

#
# The idea of the tests below is formatting 2-char string using
# % operator should not take more memory than concatenating such
# a 2-char string.
#

micropython.mem_reset()
m = func()
a + b
m_ref = func() - m
#print(m_ref)

micropython.mem_reset()
m = func()
"%s" % "ab"
m1 = func() - m
#print(m1)

vals = (a, b)
micropython.mem_reset()
m = func()
"%s%s" % vals
m2 = func() - m
#print(m2)

assert m1 - m_ref < FUZZ
assert m2 - m_ref < FUZZ
