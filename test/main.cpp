/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <iostream>

enum class ltype
{
    a = 1,
    b = 2
};

template <ltype, class>
struct x
{
};

template <class D>
struct base
{
    template <ltype L>
    using bound_type = x<L, D>;
};

template <class>
struct some
{};

template <class D>
struct derived : base<D>
{
    using base_type = base<D>;

    template <ltype L>
    using type = typename base_type::template bound_type<L>;

    template <ltype L>
    using select_type = some<type<L>>;

    template <ltype L = ltype::a>
    inline auto foo() -> select_type<L>
    {
        std::cout << "foo";
        return select_type<L>();
    }
};

struct concrete : derived<concrete>
{
};

int main(int argc, char* argv[])
{
    derived<double>::template type<ltype::a> y;
    derived<double> owner;
    owner.foo();
    return 0;
}

