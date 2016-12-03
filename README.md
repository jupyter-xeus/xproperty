# ![xproperty](http://quantstack.net/assets/images/xproperty.svg)

[![Travis](https://travis-ci.org/QuantStack/xproperty.svg?branch=master)](https://travis-ci.org/QuantStack/xproperty)
[![Appveyor](https://ci.appveyor.com/api/projects/status/vy9ej33iauvf1r8c?svg=true)](https://ci.appveyor.com/project/QuantStack/xproperty)
[![Documentation Status](http://readthedocs.org/projects/xproperty/badge/?version=latest)](https://xproperty.readthedocs.io/en/latest/?badge=latest)
[![Join the Gitter Chat](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/QuantStack/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

C++ properties and observer pattern

## Introduction

`xproperty` is a C++ library providing traitlets-style properties.

`xproperty` provides an implementation of the observer patterns relying on C++ template and preprocessor metaprogramming techniques.

Properies of observed objects have no additional memory footprint than the value they hold. The assignment of a new value is simply
replaced at compiled time by

- the call to the validator for that property
- the actual underlying assigment
- the call to the observor for that property.

We also provide the implementation of an `xobserved` class whose static validator and observer are bound to a dynamic unordered map
of callbacks that can be registered dynamically.

`xproperty` requires a modern C++ compiler supporting C++14. The following C++ compilers are supported:

- On Windows platforms, Visual C++ 2015 Update 2, or more recent
- On Unix platforms, gcc 4.9 or a recent version of Clang

## Usage

- Declaring an observed object `Foo` with two properties named `bar` and `baz` of type `double`.
- Registering a validator, executed prior to assignment, which can potentially coerce the proposed value.
- Registering a notifier, executed after the assignement.

```cpp
#include <iostream>
#include <stdexcept>

#include "xproperty/xobserved.hpp"

struct Foo : public xp::xobserved<Foo>
{
    XPROPERTY(double, Foo, bar);
    XPROPERTY(double, Foo, baz);
};
```

Registering an observer and a validator

```cpp
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
```

Testing the validated and observed properties

```cpp
foo.bar = 1.0;                           // Assigning a valid value
                                         // The notifier prints "Observer: New value of bar: 1"
std::cout << foo.bar << std::endl;       // Outputs 1.0

try
{
    foo.bar = -1.0;                      // Assigning an invalid value
}
catch (...)
{
    std::cout << foo.bar << std::endl;   // Still outputs 1.0
}
```

Shortcuts to link properties of observed objects

```cpp
// Create two observed objects
Foo source, target;
source.bar = 1.0;

// Link `source.bar` and `target.baz`
XDLINK(source, bar, target, baz);

source.bar = 2.0;
std::cout << target.baz << std::endl;    // Outputs 2.0
```

## Building and Running the Tests

Building the tests requires the [GTest](https://github.com/google/googletest) testing framework and [cmake](https://cmake.org).

gtest and cmake are available as a packages for most linux distributions. Besideds, they can also be installed with the `conda` package manager (even on windows):

```bash
conda install -c conda-forge gtest cmake
```

Once `gtest` and `cmake` are installed, you can build and run the tests:

```bash
mkdir build
cd build
cmake ..
make -j2 xtest
```

In the context of continuous integration with Travis CI, tests are run in a `conda` environment, which can be activated with

```bash
cd test
conda env create -f ./test-environment.yml
source activate test-xproperty
cd ..
cmake .
make -j2 xtest
```

## Building the HTML Documentation

xpropery's documentation is built with three tools

 - [doxygen](http://www.doxygen.org)
 - [sphinx](http://www.sphinx-doc.org)
 - [breathe](https://breathe.readthedocs.io)

While doxygen must be installed separately, you can install breathe by typing

```bash
pip install breathe
``` 

Breathe can also be installed with `conda`

```bash
conda install -c conda-forge breathe
```

Finally, build the documentation with

```bash
make html
```

from the `docs` subdirectory.

## License

We use a shared copyright model that enables all contributors to maintain the
copyright on their contributions.

This software is licensed under the BSD-3-Clause license. See the [LICENSE](LICENSE) file for details.
