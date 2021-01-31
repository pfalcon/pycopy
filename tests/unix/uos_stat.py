import uos


print(len(uos.stat("unix/uos_stat.py")))
# follow_symlinks=False
print(len(uos.stat("unix/uos_stat.py", False)))
