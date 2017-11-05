/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <utility>
#include <type_traits>

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

    using default_iterator = layout_iterator<layout_type::row_major>;
};

template <layout_type L, class A, class B>
struct select_iterator_impl
{
    using type = std::conditional_t<L == layout_type::column_major, A, B>;
};

class st {};

template <class D>
struct xcontainer : xiterable<D>
{
    using iterable_base = xiterable<D>;
    using storage_iterator = st;

    template <layout_type L>
    using layout_iterator = typename iterable_base::template layout_iterator<L>;

    template <layout_type L, class It1, class It2>
    using select_iterator_impl = std::conditional_t<L == layout_type::row_major, It1, It2>;

    template <layout_type L>
    using select_iterator = select_iterator_impl<L, storage_iterator, layout_iterator<L>>;

    template <layout_type L = layout_type::row_major>
    inline select_iterator<L> begin()
    {
        return select_iterator<L>();
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

