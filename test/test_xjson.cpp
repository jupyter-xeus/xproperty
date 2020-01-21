/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <iostream>

#include "gtest/gtest.h"

#include "xproperty/xobserved.hpp"
#include "xproperty/xjson.hpp"

struct Baz : xp::xobserved<Baz>
{
    XPROPERTY(double, Baz, bar);
};

TEST(xproperty, json)
{
    std::cout << "0" << std::endl;
    Baz foo;
    foo.bar = 2.0;
    nlohmann::json j = foo.bar;
    double t = j;
    ASSERT_EQ(2.0, t);
}
