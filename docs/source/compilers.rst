.. Copyright (c) 2017, Johan Mabille and Sylvain Corlay

   Distributed under the terms of the BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Compiler workarounds
====================

This page tracks the workarounds for the various compiler issues that we
encountered in the development. This is mostly of interest for developers
interested in contributing to xwidgets.

Visual Studio and rvalue-ness of attributes
-------------------------------------------

When `foo` is an rvalue, and `bar` is member of `foo` held by value, `foo.bar` should also be an rvalue.

This is correctly implemented in GCC (since 4.9) and Clang (since before 4.0), but not in Visual Studio.

Hence, we use a different signature for the method chaining of properties with visual studio.
