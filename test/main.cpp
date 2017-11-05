/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <iostream>

enum class layout_type
{
    row_major = 1,
    column_major = 2
};

template <class, class, layout_type>
struct xiterator
{
};

template <class D>
struct xiterable
{
    template <layout_type L>
    using layout_iterator = xiterator<D, D, L>;
};

template <layout_type L, class>
struct select_iterator_impl
{
};

template <class D>
struct xcontainer : xiterable<D>
{
    using iterable_base = xiterable<D>;

    template <layout_type L>
    using layout_iterator = typename iterable_base::template layout_iterator<L>;

    template <layout_type L>
    using select_iterator = select_iterator_impl<L, layout_iterator<L>>;

    template <layout_type L = layout_type::row_major>
    inline select_iterator<L> begin()
    {
        std::cout << "begin";
        return select_iterator<L>();
    }
};

struct concrete : xcontainer<concrete>
{
};

int main(int argc, char* argv[])
{
    xcontainer<double>::template layout_iterator<layout_type::row_major> y;
    xcontainer<double> owner;
    owner.begin();
    return 0;
}

