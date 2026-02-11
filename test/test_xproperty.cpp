/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "doctest/doctest.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

#include "test_utils.hpp"
#include "xproperty/xobserved.hpp"

struct Foo : xp::xobserved
{
    XPROPERTY(double, Foo, bar);
    XPROPERTY(double, Foo, baz);
    XPROPERTY(std::vector<std::string>, Foo, boz, {"Test"});
};

TEST_SUITE("xproperty")
{
    TEST_CASE("basic")
    {
        Foo foo;
        foo.bar = 1.0;
        REQUIRE_EQ(1.0, double(foo.bar));
        foo.bar = 2.0;
        REQUIRE_EQ(2.0, double(foo.bar));
    }

    struct Wrapper : xp::xobserved
    {
        XPROPERTY(Foo, Wrapper, foo);
    };

    TEST_CASE("nested")
    {
        Wrapper wrapper;
        wrapper.foo().bar = 1.;
        REQUIRE_EQ(1.0, double(wrapper.foo().bar));
    }

    struct Bat : xp::xobserved
    {
        XPROPERTY(double, Bat, man, 1.0);
    };

    TEST_CASE("default_values")
    {
        Bat bat;

        REQUIRE_EQ(1.0, double(bat.man));
    }

    template <class D>
    struct str_base : xp::xobserved
    {
        using derived_type = D;

        XPROPERTY(std::string, derived_type, name);
    };

    struct str_final : str_base<str_final>
    {
    };

    TEST_CASE("string_conversion")
    {
        str_final str;
        str.name = "test";
    }

    struct Ro : xp::xobserved
    {
        XPROPERTY(double, Ro, bin, 1.0, [](double& i) { if (i < 0.0) i = 0.0; });
    };

    TEST_CASE("lambda_validation")
    {
        Ro ro;
        REQUIRE_EQ(1.0, ro.bin());
        ro.bin = -1.0;
        REQUIRE_EQ(0.0, ro.bin());
    }

    template <class T>
    struct DEBUG;
}
