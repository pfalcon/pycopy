try:
    from ucollections import OrderedDict
    import uctypes
except ImportError:
    print("SKIP")
    raise SystemExit

OFFSET_BITS = 17
OFFSET_MASK = (1 << OFFSET_BITS) - 1

o = OrderedDict

desc = o((
    ("u0", uctypes.UINT8),
    ("u1", uctypes.PREV_OFFSET | uctypes.UINT16),
    ("u2", uctypes.PREV_OFFSET | uctypes.UINT32),
))

sizeof = uctypes.calc_offsets(desc, uctypes.LITTLE_ENDIAN)
print(sizeof)

sizeof = uctypes.sizeof(desc, uctypes.LITTLE_ENDIAN)
print(sizeof)

offsets = [(x[0] if isinstance(x, tuple) else x) & OFFSET_MASK for x in desc.values()]
print(offsets)


desc = o((
    ("u0", uctypes.UINT8),
    ("u1", uctypes.PREV_OFFSET | uctypes.UINT16),
    ("u2", uctypes.PREV_OFFSET | uctypes.UINT32),
    ("f1", uctypes.UINT16)
))

sizeof = uctypes.calc_offsets(desc, uctypes.LITTLE_ENDIAN)
print(sizeof)

sizeof = uctypes.sizeof(desc, uctypes.LITTLE_ENDIAN)
print(sizeof)

offsets = [(x[0] if isinstance(x, tuple) else x) & OFFSET_MASK for x in desc.values()]
print(offsets)
