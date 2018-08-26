try:
    import uctypes
except ImportError:
    print("SKIP")
    raise SystemExit

desc = {
    "f1": 0 | uctypes.UINT32,
    "f2": 4 | uctypes.UINT8,
}


# Here we assume that that we run on a platform with convential ABI
# (which rounds up structure size based on max alignment). For platforms
# where that doesn't hold, this tests should be just disabled in the runner.
print(uctypes.sizeof(desc, uctypes.NATIVE) > uctypes.sizeof(desc, uctypes.LITTLE_ENDIAN))
