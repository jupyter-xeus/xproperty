/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XOBSERVED_HPP
#define XOBSERVED_HPP

#include <cstddef>
#include <functional>
#include <type_traits>
#include <map>
#include <vector>

#include "xtl/xany.hpp"

#include "xproperty.hpp"

namespace xp
{

    // XOBSERVE(owner, Attribute, Callback)
    // Register a callback reacting to changes of the specified attribute of the owner.

    #define XOBSERVE(O, A, C) \
    O.observe(O.derived_cast().A.name(), C);

    // XUNOBSERVE(owner, Attribute)
    // Removes all callbacks reacting to changes of the specified attribute of the owner.

    #define XUNOBSERVE(O, A) \
    O.unobserve(O.derived_cast().A.name());

    // XVALIDATE(owner, Attribute, Validator)
    // Register a validator for proposed values of the specified attribute.

    #define XVALIDATE(O, A, C) \
    O.validate(O.derived_cast().A.name(), std::function<void(decltype(O)&, typename decltype(O.A)::value_type&)>(C));

    // XUNVALIDATE(owner, Attribute)
    // Removes all validators for proposed values of the specified attribute.

    #define XUNVALIDATE(O, A) \
    O.unvalidate(O.derived_cast().A.name());

    // XDLINK(Source, AttributeName, Target, AttributeName)
    // Link the value of an attribute of a source xobserved object with the value of a target object.

    #define XDLINK(S, SA, T, TA)                                                   \
    T.TA = S.SA;                                                                   \
    S.observe(S.derived_cast().SA.name(), [&S, &T](auto&) { T.TA = S.SA; });

    // XLINK(Source, AttributeName, Target, AttributeName)
    // Bidirectional link between attributes of two xobserved objects.

    #define XLINK(S, SA, T, TA)                                                    \
    T.TA = S.SA;                                                                   \
    S.observe(S.derived_cast().SA.name(), [&S, &T](const auto&) { T.TA = S.SA; });                        \
    T.observe(T.derived_cast().TA.name(), [&S, &T](const auto&) { S.SA = T.TA; });

    /*************************
     * xobserved declaration *
     *************************/

    template <class D>
    class xobserved
    {
    public:

        using derived_type = D;

        derived_type& derived_cast() noexcept;
        const derived_type& derived_cast() const noexcept;

        void observe(char const*, std::function<void(derived_type&)>);

        void unobserve(char const*);

        template <class V>
        void validate(char const*, std::function<void(derived_type&, V&)>);

        void unvalidate(char const*);

    protected:

        xobserved() = default;
        ~xobserved() = default;

        xobserved(const xobserved&) = default;
        xobserved& operator=(const xobserved&) = default;

        xobserved(xobserved&&) = default;
        xobserved& operator=(xobserved&&) = default;

    private:

        std::multimap<std::string, std::function<void(derived_type&)>> m_observers;
        std::multimap<std::string, xtl::any> m_validators;

        template <class X, class Y>
        friend class xproperty;

        template <class T>
        void notify(char const*, const T&);

        void invoke_observers(char const*);

        template <class T, class V>
        auto invoke_validators(char const*, V&& r);
    };

    template <class E>
    using is_xobserved = std::is_base_of<xobserved<E>, E>;

    /****************************
     * xobserved implementation *
     ****************************/

    template <class D>
    inline auto xobserved<D>::derived_cast() noexcept -> derived_type&
    {
        return *static_cast<derived_type*>(this);
    }

    template <class D>
    inline auto xobserved<D>::derived_cast() const noexcept -> const derived_type&
    {
        return *static_cast<const derived_type*>(this);
    }

    template <class D>
    inline void xobserved<D>::observe(char const* name, std::function<void(derived_type&)> cb)
    {
        m_observers.insert(std::make_pair(name, std::move(cb)));
    }

    template <class D>
    inline void xobserved<D>::unobserve(char const* name)
    {
        m_observers.erase(name);
    }

    template <class D>
    template <class V>
    inline void xobserved<D>::validate(char const* name, std::function<void(derived_type&, V&)> cb)
    {
        m_validators.insert(std::make_pair(name, std::move(cb)));
    }

    template <class D>
    inline void xobserved<D>::unvalidate(char const* name)
    {
        m_validators.erase(name);
    }

    template <class D>
    template <class T>
    inline void xobserved<D>::notify(char const*, const T&)
    {
    }

    template <class D>
    inline void xobserved<D>::invoke_observers(char const* name)
    {
        auto callbacks = m_observers.equal_range(name);
        for(auto it = callbacks.first; it != callbacks.second; ++it)
        {
            it->second.operator()(derived_cast());
        }
    }

    template <class D>
    template <class T, class V>
    inline auto xobserved<D>::invoke_validators(char const* name, V&& v)
    {
        using value_type = T;
        value_type value(std::forward<V>(v));

        auto callbacks = m_validators.equal_range(name);
        for(auto it = callbacks.first; it != callbacks.second; ++it)
        {
            xtl::any_cast<std::function<void(derived_type&, value_type&)>>(it->second)(derived_cast(), value);
        }

        return value;
    }
}

#endif
