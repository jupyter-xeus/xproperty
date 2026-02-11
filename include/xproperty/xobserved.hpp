/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XOBSERVED_HPP
#define XOBSERVED_HPP

#include <any>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <map>
#include <vector>

#include "xproperty.hpp"

namespace xp
{
    /*************************
     * xobserved declaration *
     *************************/

    class xobserved
    {
    public:

        template <class D>
        void observe(const char*, std::function<void(D&)>);

        void unobserve(const char*);

        template <class D, class V>
        void validate(const char*, std::function<void(D&, V&)>);

        void unvalidate(const char*);

        template <class P>
        void register_property(P& prop);

    protected:

        xobserved() = default;
        ~xobserved() = default;

        xobserved(const xobserved&) = default;
        xobserved& operator=(const xobserved&) = default;

        xobserved(xobserved&&) = default;
        xobserved& operator=(xobserved&&) = default;

    private:

        std::map<const char*, std::tuple<std::vector<std::function<void(std::any&, std::any&)>>, std::vector<std::function<void(std::any&)>>>> m_accesses;

        template <class X, class Y>
        friend class xproperty;

        template <class T>
        void notify(const char*, const T&);

        void invoke_observers(const char*, std::any& owner);

        template <class T, class V>
        auto invoke_validators(const char*, std::any& owner, V&& r);

        void observe_impl(const char*, std::function<void(std::any&)>);
        void validate_impl(const char*, std::function<void(std::any&, std::any&)>);
    };

    template <class E>
    using is_xobserved = std::is_base_of<xobserved, E>;

    /****************************
     * xobserved implementation *
     ****************************/

    template <class D>
    inline void xobserved::observe(const char* name,  std::function<void(D&)> observer)
    {
        observe_impl(name, [observer = std::move(observer)](std::any& owner)
        {
            observer(std::any_cast<std::reference_wrapper<D>>(owner).get());
        });
    }

    inline void xobserved::unobserve(const char* name)
    {
        std::get<1>(m_accesses[name]).clear();
    }

    template <class D, class V>
    inline void xobserved::validate(const char* name, std::function<void(D&, V&)> validator)
    {
        validate_impl(name, [validator = std::move(validator)](std::any& owner, std::any& proposal)
        {
            validator(std::any_cast<std::reference_wrapper<D>>(owner).get(), std::any_cast<V&>(proposal));
        });
    }

    inline void xobserved::unvalidate(const char* name)
    {
        std::get<0>(m_accesses[name]).clear();
    }

    template <class P>
    void xobserved::register_property(P& prop)
    {
    }

    template <class T>
    inline void xobserved::notify(const char*, const T&)
    {
    }

    inline void xobserved::invoke_observers(const char* name, std::any& owner)
    {
        for(auto& observer : std::get<1>(m_accesses[name]))
        {
            observer(owner);
        }
    }

    template <class T, class V>
    inline auto xobserved::invoke_validators(const char* name, std::any& owner, V&& v)
    {
        using value_type = T;
        value_type value(std::forward<V>(v));
        std::any value_any = value;
        for(auto& validator : std::get<0>(m_accesses[name]))
        {
            validator(owner, value_any);
        }
        return std::any_cast<value_type>(value_any);
    }

    inline void xobserved::observe_impl(const char* name, std::function<void(std::any&)> cb)
    {
        std::get<1>(m_accesses[name]).emplace_back(std::move(cb));
    }

    inline void xobserved::validate_impl(const char* name, std::function<void(std::any&, std::any&)> cb)
    {
        std::get<0>(m_accesses[name]).emplace_back(std::move(cb));
    }
}

#endif
