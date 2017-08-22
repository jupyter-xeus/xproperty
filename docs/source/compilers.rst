.. Copyright (c) 2016, Johan Mabille and Sylvain Corlay

   Distributed under the terms of the BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Compiler workarounds
====================

This page tracks the workarounds for the various compiler issues that we encountered in the development. This is mostly of interest for developers interested in contributing to xtensor.

Visual Studio and internal classes
----------------------------------

In ``xproperty`` properties are internal classes that implement a CRTP pattern. Unlike most CRTP patterns implemented in QuantStack projects, the internal ``typedef`` to the derived class is not called ``derived_type`` but ``xp_derived_type``. The reason for this, is that the way MSVC resolves typedefs of internal classes is bugged: if the owner itself has a ``derived_type`` typedef, its value is going to be used instead.

In other words, the outer class typedefs mask the inner class typedefs.

