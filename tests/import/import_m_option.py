# Check handling of "-m" option support on import side.

__import__("import_m", None, None, False)

__import__("pkg_m.pkg2", None, None, False)
__import__("pkg_m.pkg2.__main__", None, None, False)
__import__("pkg_m.pkg2.mod", None, None, False)
