/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "gtest/gtest.h"

#include <iostream>

#include <stdexcept>

#include "xproperty/xobserved.hpp"

struct Foo
{
    MAKE_OBSERVED()

    XPROPERTY(double, Foo, bar);
    XPROPERTY(double, Foo, baz);
};

XVALIDATE_STATIC(double, Foo, bar, proposal)
{
    std::cout << "Validator: Proposal: " << proposal << std::endl;
    if (proposal < 0.0)
    {
        throw std::runtime_error("Only non-negative values are valid.");
    }
    return proposal;
}

XOBSERVE_STATIC(double, Foo, bar)
{
    std::cout << "Observer: New value of bar: " << bar << std::endl;
};

TEST(xproperty, basic)
{
    Foo foo;

    foo.bar = 1.0;
    ASSERT_EQ(1.0, foo.bar);
    // ASSERT_THROW({ foo.bar = -1.0; }, std::runtime_error);
    // ASSERT_EQ(1.0, foo.bar);
}
