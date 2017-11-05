/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <utility>
#include <type_traits>


    struct identity_functor
    {
        template <class T>
        T&& operator()(T&& x) const
        {
            return std::forward<T>(x);
        }
    };

    template <class TF, class FF>
    auto static_if(std::true_type, const TF& tf, const FF&)
    {
        return tf(identity_functor());
    }

    template <class TF, class FF>
    auto static_if(std::false_type, const TF&, const FF& ff)
    {
        return ff(identity_functor());
    }

    template <bool cond, class TF, class FF>
    auto static_if(const TF& tf, const FF& ff)
    {
        return static_if(std::integral_constant<bool, cond>(), tf, ff);
    }













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
        return static_if<L == layout_type::row_major>([&](auto self)
        {
            return select_iterator<L>();
        }, /*else*/ [&](auto self)
        {
            return select_iterator<L>();
        });
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

