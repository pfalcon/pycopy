Pycopy Documentation
====================

The Pycopy documentation can be found at: https://pycopy.readthedocs.io/ .

The documentation you see there is generated from the files in the docs tree:
https://github.com/pfalcon/pycopy/tree/master/docs

Building the documentation locally
----------------------------------

If you're making changes to the documentation, you may want to build the
documentation locally so that you can preview your changes.

Install Sphinx, and optionally (for the RTD-styling), sphinx_rtd_theme,
preferably in a virtualenv:

     pip install sphinx
     pip install sphinx_rtd_theme

In `docs/`, build the docs:

    make html

You'll find the index page at `docs/build/html/index.html`.

PDF manual generation
---------------------

This can be achieved with:

    make latexpdf

but require rather complete install of LaTeX with various extensions. On
Debian/Ubuntu, try (500MB+ download):

    apt-get install texlive-latex-recommended texlive-latex-extra
