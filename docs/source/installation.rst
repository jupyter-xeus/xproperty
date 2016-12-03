.. Copyright (c) 2016, Johan Mabille and Sylvain Corlay

   Distributed under the terms of the BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Installation
============

`xproperty` is a header-only library. We provide a package for the conda package manager.

.. code::

    conda install -c conda-forge xproperty 

Or you can directly install it from the sources:

.. code::

    cmake -D CMAKE_INSTALL_PREFIX=your_install_prefix
    make install
