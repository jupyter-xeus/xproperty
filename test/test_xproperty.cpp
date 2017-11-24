/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "gtest/gtest.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

#include "test_utils.hpp"
#include "xproperty/xobserved.hpp"

struct Foo
{
    MAKE_OBSERVED()

    XPROPERTY(double, Foo, bar);
    XPROPERTY(double, Foo, baz);
};

XVALIDATE_STATIC(double, Foo, bar, proposal)
{
    ++xp::get_validate_count();
    if (proposal < 0.0)
    {
        throw std::runtime_error("Only non-negative values are valid.");
    }
    return proposal;
}

XOBSERVE_STATIC(double, Foo, bar)
{
    ++xp::get_observe_count();
};

TEST(xproperty, basic)
{
    xp::reset_counter();
    Foo foo;

    foo.bar = 1.0;
    ASSERT_EQ(1.0, double(foo.bar));
    ASSERT_EQ(1, xp::get_observe_count());
    ASSERT_EQ(1, xp::get_validate_count());
    ASSERT_THROW({ foo.bar = -1.0; }, std::runtime_error);
    ASSERT_EQ(1.0, double(foo.bar));
    ASSERT_EQ(1, xp::get_observe_count());
    ASSERT_EQ(2, xp::get_validate_count());
}

struct Wrapper
{
    MAKE_OBSERVED()

    XPROPERTY(Foo, Wrapper, foo);
};

TEST(xproperty, nested)
{
    xp::reset_counter();
    Wrapper wrapper;

    wrapper.foo().bar = 1.;
    ASSERT_EQ(1.0, double(wrapper.foo().bar));
    ASSERT_EQ(1, xp::get_observe_count());
    ASSERT_EQ(1, xp::get_validate_count());
}

struct Bat
{
    MAKE_OBSERVED()

    XPROPERTY(double, Bat, man, 1.0);
};

TEST(xproperty, default_values)
{
    Bat bat;

    ASSERT_EQ(1.0, double(bat.man));
}

template <class D>
struct str_base
{
    using derived_type = D;
    MAKE_OBSERVED()

    XPROPERTY(std::string, derived_type, name);
};

struct str_final : str_base<str_final>
{
};

TEST(xproperty, string_conversion)
{
    str_final str;
    str.name = "test";
}

struct Ro
{
    MAKE_OBSERVED()

    XPROPERTY(double, Ro, bin, 1.0, [](double& i) { if (i < 0.0) i = 0.0; });
};

TEST(xproperty, lambda_validation)
{
    Ro ro;
    ASSERT_EQ(1.0, ro.bin());
    ro.bin = -1.0;
    ASSERT_EQ(0.0, ro.bin());
}
