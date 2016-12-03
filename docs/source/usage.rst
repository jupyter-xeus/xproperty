.. Copyright (c) 2016, Johan Mabille and Sylvain Corlay

   Distributed under the terms of the BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Usage
=====

Basic Usage
-----------

- Declaring an observed object ``Foo`` with two properties named ``bar`` and ``baz`` of type `double`.
- Registering a validator, executed prior to assignment, which can potentially coerce the proposed value.
- Registering a notifier, executed after the assignement.

.. code::

    #include <iostream>
    #include <stdexcept>

    #include "xproperty/xobserved.hpp"

    struct Foo : public xp::xobserved<Foo>
    {
        XPROPERTY(double, Foo, bar);
        XPROPERTY(double, Foo, baz);
    };

Registering an observer and a validator

.. code::

    Foo foo;

    XOBSERVE(foo, bar, [](const Foo& f) 
    {
        std::cout << "Observer: New value of bar: " << f.bar << std::endl;
    });

    XVALIDATE(foo, bar, [](const Foo&, double proposal) 
    {
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

    // Link `source.bar` and `target.baz`
    XDLINK(source, bar, target, baz);

    source.bar = 2.0;
    std::cout << target.baz << std::endl;    // Outputs 2.0


Advanced Usage: Using `XPROPERTY` without `xobserved`
-----------------------------------------------------

The standard usage of ``XPROPERTY``, with the CRTP inheritance from ``xobserved`` is not optimal
if you known at build time what validator and notifiers should be called.

Indeed, the ``xobserved``, which allows the dynamic registration of validators and notifiers stores those in
unordered maps, which

 - increases the memory footprint of the observed object
 - results in the ``O(1)`` lookup into the hash map.

Instead, you can use ``XPROPERTY`` alone, to remove that overhead.

.. code::

    #include <iostream>
    #include <stdexcept>

    #include "xproperty/xproperty.hpp"

    struct Foo
    {
        XPROPERTY(double, Foo, bar);

        MAKE_OBSERVED();

        XVALIDATE_STATIC(double, Foo, bar, proposal) 
        {
            std::cout << "Validator: Proposal: " << proposal << std::endl;
            if (proposal < 0)
            {
                throw std::runtime_error("Only non-negative values are valid.");
            }
            return proposal;
        });

        XOBSERVE_STATIC(double, Foo, bar) 
        {
            std::cout << "Observer: New value of bar: " << f.bar << std::endl;
        });
    }

In this case, the assignment of the ``Foo::bar`` property will simply be replaced by the calls to the validator
and notifier at build time without any overhead in memory footprint.

- Unlike dynamic validators and notifiers, static validators and notifiers are methods of the observed objects.
- One cannot mix the static and dynamic validators and observers
