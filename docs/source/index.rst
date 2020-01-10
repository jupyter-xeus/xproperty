.. Copyright (c) 2016, Johan Mabille and Sylvain Corlay

   Distributed under the terms of the BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

.. image:: xproperty.svg
   :alt: xproperty

C++ properties and observer pattern

Introduction
------------

``xproperty`` is a C++ library providing traitlets-style properties.

``xproperty`` provides an implementation of the observer patterns relying on C++ template and preprocessor metaprogramming techniques.

Properies of observed objects have no additional memory footprint than the value they hold. The assignment of a new value is simply
replaced at compiled time by

- the call to the validator for that property
- the actual underlying assigment
- the call to the observor for that property.

We also provide the implementation of an ``xobserved`` class whose static validator and observer are bound to a dynamic unordered map
of callbacks that can be registered dynamically.

``xproperty`` requires a modern C++ compiler supporting C++14. The following C++ compilers are supported:

- On Windows platforms, Visual C++ 2015 Update 2, or more recent
- On Unix platforms, gcc 4.9 or a recent version of Clang

Licensing
---------

We use a shared copyright model that enables all contributors to maintain the
copyright on their contributions.

This software is licensed under the BSD-3-Clause license. See the LICENSE file for details.

.. toctree::
   :caption: INSTALLATION
   :maxdepth: 2

   installation

.. toctree::
   :caption: USAGE
   :maxdepth: 2

   usage
