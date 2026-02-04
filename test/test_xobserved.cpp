/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "doctest/doctest.h"

#include <any>
#include <cstddef>
#include <iostream>
#include <stdexcept>

#include "test_utils.hpp"

#include "xproperty/xobserved.hpp"

struct Observed : public xp::xobserved
{
    XPROPERTY(double, Observed, bar);
    XPROPERTY(double, Observed, baz);
};

TEST_SUITE("xobserved")
{
    TEST_CASE("basic")
    {
        xp::reset_counter();
        Observed foo;

        foo.observe<Observed>(foo.bar.name(), [](Observed&) {
            ++xp::get_observe_count();
        });

        // Validator refusing negative values
        foo.validate<Observed, double>(foo.bar.name(), [](Observed&, double& proposal) {
            ++xp::get_validate_count();
            if (proposal < 0.0)
            {
                throw std::runtime_error("Only non-negative values are valid.");
            }
        });

        foo.bar = 1.0;
        REQUIRE_EQ(1.0, double(foo.bar));
        REQUIRE_EQ(size_t(1), xp::get_observe_count());
        REQUIRE_EQ(size_t(1), xp::get_validate_count());
        REQUIRE_THROWS_AS({ foo.bar = -1.0; }, std::runtime_error);
        REQUIRE_EQ(1.0, double(foo.bar));
        REQUIRE_EQ(size_t(1), xp::get_observe_count());
        REQUIRE_EQ(size_t(2), xp::get_validate_count());

        foo.unvalidate(foo.bar.name());
        foo.bar = -1.0;
        REQUIRE_EQ(-1.0, double(foo.bar));
        REQUIRE_EQ(size_t(2), xp::get_observe_count());
        REQUIRE_EQ(size_t(2), xp::get_validate_count());

        // validator coercing values to be non-positive
        foo.validate<Observed, double>(foo.bar.name(), [](Observed&, double& proposal) {
            ++xp::get_validate_count();
            if (proposal > 0)
            {
                proposal = 0.0;
            }
        });

        foo.bar = 1.0;
        REQUIRE_EQ(0.0, double(foo.bar));
        REQUIRE_EQ(size_t(3), xp::get_observe_count());
        REQUIRE_EQ(size_t(3), xp::get_validate_count());
    }

    TEST_CASE("links")
    {
        xp::reset_counter();
        Observed source, target;

        source.bar = 1.0;
        target.baz = source.bar;
        source.observe<Observed>(source.bar.name(), [&](auto&) { target.baz = source.bar; });
        REQUIRE_EQ(1.0, double(target.baz));
        source.bar = 2.0;
        REQUIRE_EQ(2.0, double(target.baz));
    }

    TEST_CASE("value_semantic")
    {
        Observed foo1, foo2;

        foo1.observe<Observed>(foo1.bar.name(), [](Observed&) {
            ++xp::get_observe_count();
        });

        foo1.bar = 2.5;
        foo2.bar = 4.5;

        xp::reset_counter();
        foo1 = foo2;

        REQUIRE_EQ(double(foo2.bar), double(foo1.bar));
        REQUIRE_EQ(size_t(0), xp::get_observe_count());
    }
}
