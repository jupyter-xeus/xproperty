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

struct Observed : public xp::xobserved<Observed>
{
    XPROPERTY(double, Observed, bar);
    XPROPERTY(double, Observed, baz);
};

TEST(xobserved, basic)
{
    Observed foo;

    XOBSERVE(foo, bar, [](const Observed& f) 
    {
        std::cout << "Observer: New value of bar: " << f.bar << std::endl;
    });

    // Validator refusing negative values
    XVALIDATE(foo, bar, [](const Observed&, double proposal) 
    {
        std::cout << "Validator: Proposal: " << proposal << std::endl;
        if (proposal < 0.0)
        {
            throw std::runtime_error("Only non-negative values are valid.");
        }
        return proposal;
    });

    foo.bar = 1.0;
    ASSERT_EQ(1.0, foo.bar);
    ASSERT_THROW({ foo.bar = -1.0; }, std::runtime_error);
    ASSERT_EQ(1.0, foo.bar);
    
    XUNVALIDATE(foo, bar);
    foo.bar = -1.0;
    ASSERT_EQ(-1.0, foo.bar);
    
    // validator coercing values to be non-positive
    XVALIDATE(foo, bar, [](const Observed&, double proposal) 
    {
        if (proposal > 0)
            return 0.0;
        return proposal;
    });
    
    foo.bar = 1.0;
    ASSERT_EQ(0.0, foo.bar);
}

TEST(xobserved, links)
{
    Observed source, target;
    source.bar = 1.0;
    XDLINK(source, bar, target, baz);
    ASSERT_EQ(1.0, target.baz);
    source.bar = 2.0;
    ASSERT_EQ(2.0, target.baz);
}
