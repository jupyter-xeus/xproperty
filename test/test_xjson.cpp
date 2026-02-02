/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "doctest/doctest.h"

#include <iostream>

#include "xproperty/xobserved.hpp"
#include "xproperty/xjson.hpp"

struct Baz : xp::xobserved
{
    XPROPERTY(double, Baz, bar);
};

TEST_SUITE("xproperty_json")
{
    TEST_CASE("json")
    {
        std::cout << "0" << std::endl;
        Baz foo;
        foo.bar = 2.0;
        nlohmann::json j = foo.bar;
        double t = j;
        REQUIRE_EQ(2.0, t);
    }
}

