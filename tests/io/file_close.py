f = open("io/data/file1")
f.close()
# Should be possible to close again without error.
f.close()
