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

    // XOBSERVE(owner, Attribute, Callback)
    // Register a callback reacting to changes of the specified attribute of the owner.

    #define XOBSERVE(O, A, C) \
    O.observe(O.A.name(), C);

    // XUNOBSERVE(owner, Attribute)
    // Removes all callbacks reacting to changes of the specified attribute of the owner.

    #define XUNOBSERVE(O, A) \
    O.unobserve(O.A.name());

    // XVALIDATE(owner, Attribute, Validator)
    // Register a validator for proposed values of the specified attribute.

    #define XVALIDATE(O, A, C) \
    O.validate(O.A.name(), C);

    // XUNVALIDATE(owner, Attribute)
    // Removes all validators for proposed values of the specified attribute.

    #define XUNVALIDATE(O, A) \
    O.unvalidate(O.A.name());

    // XDLINK(Source, AttributeName, Target, AttributeName)
    // Link the value of an attribute of a source xobserved object with the value of a target object.

    #define XDLINK(S, SA, T, TA)                                                   \
    T.TA = S.SA;                                                                   \
    S.observe(S.SA.name(), [&](const std::any&) { T.TA = S.SA; });

    // XLINK(Source, AttributeName, Target, AttributeName)
    // Bidirectional link between attributes of two xobserved objects.

    #define XLINK(S, SA, T, TA)                                                    \
    T.TA = S.SA;                                                                   \
    S.observe(S.SA.name(), [&](const std::any&) { T.TA = S.SA; }); \
    T.observe(T.TA.name(), [&](const std::any&) { S.SA = T.TA; });

    /*************************
     * xobserved declaration *
     *************************/

    class xobserved
    {
    public:

        void observe(const char*, std::function<void(const std::any&)>);

        void unobserve(const char*);

        void validate(const char*, std::function<void(std::any, std::any&)>);

        void unvalidate(const char*);

    protected:

        xobserved() = default;
        ~xobserved() = default;

        xobserved(const xobserved&) = default;
        xobserved& operator=(const xobserved&) = default;

        xobserved(xobserved&&) = default;
        xobserved& operator=(xobserved&&) = default;

    private:

        std::map<const char*, std::tuple<std::vector<std::function<void(std::any, std::any&)>>, std::vector<std::function<void(const std::any&)>>>> m_accesses;

        template <class X, class Y>
        friend class xproperty;

        template <class T>
        void notify(const char*, const T&);

        void invoke_observers(const char*, const std::any& owner);

        template <class T, class V>
        auto invoke_validators(const char*, std::any owner, V&& r);
    };

    template <class E>
    using is_xobserved = std::is_base_of<xobserved, E>;

    /****************************
     * xobserved implementation *
     ****************************/

    inline void xobserved::observe(const char* name, std::function<void(const std::any&)> cb)
    {
        std::get<1>(m_accesses[name]).emplace_back(std::move(cb));
    }

    inline void xobserved::unobserve(const char* name)
    {
        std::get<1>(m_accesses[name]).clear();
    }


    inline void xobserved::validate(const char* name, std::function<void(std::any, std::any&)> cb)
    {
        std::get<0>(m_accesses[name]).emplace_back(std::move(cb));
    }

    inline void xobserved::unvalidate(const char* name)
    {
        std::get<0>(m_accesses[name]).clear();
    }

    template <class T>
    inline void xobserved::notify(const char*, const T&)
    {
    }

    inline void xobserved::invoke_observers(const char* name, const std::any& owner)
    {
        for(auto& observer : std::get<1>(m_accesses[name]))
        {
            observer(owner);
        }
    }

    template <class T, class V>
    inline auto xobserved::invoke_validators(const char* name, std::any owner, V&& v)
    {
        using value_type = T;
        value_type value(std::forward<V>(v));
        std::any value_any = value;
        for(auto& validator : std::get<0>(m_accesses[name]))
        {
            validator(std::move(owner), value_any);
        }
        return std::any_cast<value_type>(value_any);
    }
}

#endif
