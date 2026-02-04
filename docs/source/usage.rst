.. Copyright (c) 2016, Johan Mabille and Sylvain Corlay

   Distributed under the terms of the BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Usage
=====

Basic Usage
-----------

- Declaring an observed object ``Foo`` with two properties named ``bar`` and ``baz`` of type `double`.
- Registering a validator, executed prior to assignment, which can potentially coerce the proposed value.
- Registering a notifier, executed after the assignment.

.. code::

    #include <iostream>
    #include <stdexcept>
    #include <string>

    #include "xproperty/xobserved.hpp"

    struct Foo : public xp::xobserved<Foo>
    {
        XPROPERTY(double, Foo, bar);
        XPROPERTY(std::string, Foo, baz);
    };

Registering an observer and a validator

.. code::

    Foo foo;

    foo.observe<Foo>(foo.bar.name(), [](const Foo& f) {
        std::cout << "Observer: New value of bar: " << f.bar << std::endl;
    });

    foo.validate<Foo, double>(foo.bar.name(), [](Foo&, double& proposal) {
        std::cout << "Validator: Proposal: " << proposal << std::endl;
        if (proposal < 0)
        {
            throw std::runtime_error("Only non-negative values are valid.");
        }
        return proposal;
    });

Testing the validated and observed properties

.. code::

    foo.bar = 1.0;                          // Assigning a valid value
                                            // The notifier prints "Observer: New value of bar: 1"
    std::cout << foo.bar << std::endl;      // Outputs 1.0

    try
    {
        foo.bar = -1.0;                     // Assigning an invalid value
    }
    catch (...)
    {
        std::cout << foo.bar << std::endl;  // Still outputs 1.0
    }

Shortcuts to link properties of observed objects

.. code::

    // Create two observed objects
    Foo source, target;
    source.bar = 1.0;

    // Link `source.bar` and `target.bar`
    target.bar = source.bar;
    source.observe<Foo>(source.bar.name(), [&](auto&) { target.bar = source.bar; });

    source.bar = 2.0;
    std::cout << target.bar << std::endl;    // Outputs 2.0

Out-of-order initialization of properties

.. code::

    auto foo = Foo()
        .baz("hello, world");

    std::cout << foo.baz << std::endl;       // Outputs hello, world

