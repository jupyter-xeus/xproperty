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

struct Foo : xp::xobserved<Foo>
{
    XPROPERTY(double, Foo, bar);
    XPROPERTY(double, Foo, baz);
    XPROPERTY(std::vector<std::string>, Foo, boz, {"Test"});
};

TEST(xproperty, basic)
{
    Foo foo;
    foo.bar = 1.0;
    ASSERT_EQ(1.0, double(foo.bar));
    foo.bar = 2.0;
    ASSERT_EQ(2.0, double(foo.bar));
}

struct Wrapper : xp::xobserved<Wrapper>
{
    XPROPERTY(Foo, Wrapper, foo);
};

TEST(xproperty, nested)
{
    Wrapper wrapper;
    wrapper.foo().bar = 1.;
    ASSERT_EQ(1.0, double(wrapper.foo().bar));
}

struct Bat : xp::xobserved<Bat>
{
    XPROPERTY(double, Bat, man, 1.0);
};

TEST(xproperty, default_values)
{
    Bat bat;

    ASSERT_EQ(1.0, double(bat.man));
}

template <class D>
struct str_base : xp::xobserved<D>
{
    using derived_type = D;

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

struct Ro : xp::xobserved<Ro>
{
    XPROPERTY(double, Ro, bin, 1.0, [](Ro&, double& i) { if (i < 0.0) i = 0.0; });
};

TEST(xproperty, lambda_validation)
{
    Ro ro;
    ASSERT_EQ(1.0, ro.bin());
    ro.bin = -1.0;
    ASSERT_EQ(0.0, ro.bin());
}
