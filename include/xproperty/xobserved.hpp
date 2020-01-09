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
#include <unordered_map>
#include <vector>

#include "xtl/xany.hpp"

#include "xproperty.hpp"

namespace xp
{

    // XOBSERVE(owner, Attribute, Callback)
    // Register a callback reacting to changes of the specified attribute of the owner.

    #define XOBSERVE(O, A, C) \
    O.observe(#A, C);

    // XUNOBSERVE(owner, Attribute)
    // Removes all callbacks reacting to changes of the specified attribute of the owner.

    #define XUNOBSERVE(O, A) \
    O.unobserve(#A);

    // XVALIDATE(owner, Attribute, Validator)
    // Register a validator for proposed values of the specified attribute.

    #define XVALIDATE(O, A, C) \
    O.validate(#A, std::function<void(decltype(O)&, typename decltype(O.A)::value_type&)>(C));

    // XUNVALIDATE(owner, Attribute)
    // Removes all validators for proposed values of the specified attribute.

    #define XUNVALIDATE(O, A) \
    O.unvalidate(#A);

    // XDLINK(Source, AttributeName, Target, AttributeName)
    // Link the value of an attribute of a source xobserved object with the value of a target object.

    #define XDLINK(S, SA, T, TA)                                                   \
    T.TA = S.SA;                                                                   \
    S.observe(#SA, [&S, &T](auto&) { T.TA = S.SA; });

    // XLINK(Source, AttributeName, Target, AttributeName)
    // Bidirectional link between attributes of two xobserved objects.

    #define XLINK(S, SA, T, TA)                                                    \
    T.TA = S.SA;                                                                   \
    S.observe(#SA, [&S, &T](const auto&) { T.TA = S.SA; });                        \
    T.observe(#TA, [&S, &T](const auto&) { S.SA = T.TA; });

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

        void observe(const std::string&, std::function<void(derived_type&)>);

        void unobserve(const std::string&);

        template <class V>
        void validate(const std::string&, std::function<void(derived_type&, V&)>);

        void unvalidate(const std::string&);

    protected:

        xobserved() = default;
        ~xobserved() = default;

        xobserved(const xobserved&) = default;
        xobserved& operator=(const xobserved&) = default;

        xobserved(xobserved&&) = default;
        xobserved& operator=(xobserved&&) = default;

    private:

        std::unordered_map<std::string, std::vector<std::function<void(derived_type&)>>> m_observers;
        std::unordered_map<std::string, std::vector<xtl::any>> m_validators;

        template <class X, class Y>
        friend class xproperty;

        template <class T>
        void notify(const std::string&, const T&);

        void invoke_observers(const std::string&);

        template <class P, class V>
        auto invoke_validators(const std::string&, V&& r);
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
    inline void xobserved<D>::observe(const std::string& name, std::function<void(derived_type&)> cb)
    {
        auto position = m_observers.find(name);
        if (position == m_observers.end())
        {
            m_observers[name] = {std::move(cb)};
        }
        else
        {
            position->second.push_back(std::move(cb));
        }
    }

    template <class D>
    inline void xobserved<D>::unobserve(const std::string& name)
    {
        m_observers.erase(name);
    }

    template <class D>
    template <class V>
    inline void xobserved<D>::validate(const std::string& name, std::function<void(derived_type&, V&)> cb)
    {
        auto position = m_validators.find(name);
        if (position == m_validators.end())
        {
            m_validators[name] = {std::move(cb)};
        }
        else
        {
            position->second.push_back(std::move(cb));
        }
    }

    template <class D>
    inline void xobserved<D>::unvalidate(const std::string& name)
    {
        m_validators.erase(name);
    }

    template <class D>
    template <class T>
    inline void xobserved<D>::notify(const std::string&, const T&)
    {
    }

    template <class D>
    inline void xobserved<D>::invoke_observers(const std::string& name)
    {
        auto position = m_observers.find(name);
        if (position != m_observers.end())
        {
            const auto& callbacks = position->second;
            for (auto it = callbacks.cbegin(); it != callbacks.cend(); ++it)
            {
                it->operator()(derived_cast());
            }
        }
    }

    template <class D>
    template <class P, class V>
    inline auto xobserved<D>::invoke_validators(const std::string& name, V&& v)
    {
        using value_type = typename P::value_type;
        value_type value(std::forward<V>(v));

        auto position = m_validators.find(name);
        if (position != m_validators.end())
        {
            const auto& callbacks = position->second;
            for (auto it = callbacks.cbegin(); it != callbacks.cend(); ++it)
            {
                xtl::any_cast<std::function<void(derived_type&, value_type&)>>(*it)(derived_cast(), value);
            }
        }

        return value;
    }
}

#endif
