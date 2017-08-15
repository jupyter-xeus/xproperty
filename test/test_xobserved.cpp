/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "gtest/gtest.h"

#include <iostream>
#include <cstddef>
#include <stdexcept>
#include "test_utils.hpp"
#include "xproperty/xobserved.hpp"

struct Observed : public xp::xobserved<Observed>
{
    XPROPERTY(double, Observed, bar);
    XPROPERTY(double, Observed, baz);
};

TEST(xobserved, basic)
{
    xp::reset_counter();
    Observed foo;

    XOBSERVE(foo, bar, [](const Observed&) 
    {
        ++xp::get_observe_count();
    });

    // Validator refusing negative values
    XVALIDATE(foo, bar, [](const Observed&, double proposal) 
    {
        ++xp::get_validate_count();
        if (proposal < 0.0)
        {
            throw std::runtime_error("Only non-negative values are valid.");
        }
        return proposal;
    });

    foo.bar = 1.0;
    ASSERT_EQ(1.0, double(foo.bar));
    ASSERT_EQ(1, xp::get_observe_count());
    ASSERT_EQ(1, xp::get_validate_count());
    ASSERT_THROW({ foo.bar = -1.0; }, std::runtime_error);
    ASSERT_EQ(1.0, double(foo.bar));
    ASSERT_EQ(1, xp::get_observe_count());
    ASSERT_EQ(2, xp::get_validate_count());
    
    XUNVALIDATE(foo, bar);
    foo.bar = -1.0;
    ASSERT_EQ(-1.0, double(foo.bar));
    ASSERT_EQ(2, xp::get_observe_count());
    ASSERT_EQ(2, xp::get_validate_count());
    
    // validator coercing values to be non-positive
    XVALIDATE(foo, bar, [](const Observed&, double proposal) 
    {
        ++xp::get_validate_count();
        if (proposal > 0)
            return 0.0;
        return proposal;
    });
    
    foo.bar = 1.0;
    ASSERT_EQ(0.0, double(foo.bar));
    ASSERT_EQ(3, xp::get_observe_count());
    ASSERT_EQ(3, xp::get_validate_count());
}

TEST(xobserved, links)
{
    xp::reset_counter();
    Observed source, target;

    source.bar = 1.0;
    XDLINK(source, bar, target, baz);
    ASSERT_EQ(1.0, double(target.baz));
    source.bar = 2.0;
    ASSERT_EQ(2.0, double(target.baz));
}
