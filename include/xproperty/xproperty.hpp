/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XPROPERTY_HPP
#define XPROPERTY_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

#include "xtl/xfunctional.hpp"

namespace xp
{
    /*************************
     * xproperty declaration *
     *************************/

    // Type, Owner Type, Derived Type

    template <class T, class O>
    class xproperty
    {
    public:

        using xp_owner_type = O;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;

        constexpr xproperty(xp_owner_type* owner, const std::string& name) noexcept(noexcept(std::is_nothrow_constructible<value_type>::value));
        template <class V>
        constexpr xproperty(xp_owner_type* owner, const std::string& name, V&& value) noexcept(noexcept(std::is_nothrow_constructible<value_type>::value));

        operator reference() noexcept;
        operator const_reference() const noexcept;

        reference operator()() noexcept;
        const_reference operator()() const noexcept;

        xp_owner_type operator()(const value_type& arg) && noexcept;
        xp_owner_type operator()(value_type&& arg) && noexcept;

        template <class Arg, class... Args>
        xp_owner_type operator()(Arg&& arg, Args&&... args) && noexcept;

        const std::string& name() const noexcept;

        template <class V>
        reference operator=(V&&);

    private:

        xp_owner_type* owner() noexcept;

        xp_owner_type* m_owner;
        std::string m_name;
        value_type m_value;
    };

    /********************************************************
     * XPROPERTY, XDEFAULT_VALUE, XDEFAULT_GENERATOR macros *
     ********************************************************/

    // XPROPERTY(Type, Owner, Name)
    // XPROPERTY(Type, Owner, Name, Value)
    // XPROPERTY(Type, Owner, Name, Value, Validator)
    //
    // Defines a property of the specified type and name, for the specified owner type.
    //
    // The owner type must have two methods
    //
    //  - template <class P, class V>
    //    auto invoke_validators(const std::string& name, V&& proposal) const;
    //  - void invoke_observers(const std::string& name) const;
    //
    // The `T` typename is a universal reference on the proposed value.
    // The return type of `invoke_validator` must be convertible to the value_type of the property.

    // lambda_validator(rhs);                                                           

    #define XPROPERTY_GENERAL(T, O, D, DEFAULT_VALUE, lambda_validator)                          \
    ::xp::xproperty<T, O> D = ::xp::xproperty<T, O>((O*)this, #D, T(DEFAULT_VALUE));

    #define XPROPERTY_NODEFAULT(T, O, D)                                                         \
    XPROPERTY_GENERAL(T, O, D, T(), xtl::identity())

    #define XPROPERTY_DEFAULT(T, O, D, V)                                                        \
    XPROPERTY_GENERAL(T, O, D, V, xtl::identity())

    #define XPROPERTY_OVERLOAD(_1, _2, _3, _4, _5, NAME, ...) NAME

    #ifdef _MSC_VER
    // Workaround for MSVC not expanding macros
    #define XPROPERTY_EXPAND(x) x
    #define XPROPERTY(...) XPROPERTY_EXPAND(XPROPERTY_OVERLOAD(__VA_ARGS__, XPROPERTY_GENERAL, XPROPERTY_DEFAULT, XPROPERTY_NODEFAULT)(__VA_ARGS__))
    #else
    #define XPROPERTY(...) XPROPERTY_OVERLOAD(__VA_ARGS__, XPROPERTY_GENERAL, XPROPERTY_DEFAULT, XPROPERTY_NODEFAULT)(__VA_ARGS__)
    #endif

    /***********************
     * MAKE_OBSERVED macro *
     ***********************/

    // MAKE_OBSERVED()
    //
    // Adds the required boilerplate for an obsered structure.

    #define MAKE_OBSERVED()                                                                 \
    template <class P>                                                                      \
    inline void notify(const P&) const {}                                                   \
                                                                                            \
    inline void invoke_observers(const std::string&) const {}                               \
                                                                                            \
    template <class P, class V>                                                             \
    inline auto invoke_validators(const std::string&, V&& r) const { return r; }

    /****************************
     * xproperty implementation *
     ****************************/

    template <class T, class O>
    inline constexpr xproperty<T, O>::xproperty(xp_owner_type* owner, const std::string& name) noexcept(noexcept(std::is_nothrow_constructible<value_type>::value))
        : m_owner(owner), m_name(name), m_value()
    {
    }

    template <class T, class O>
    template <class V>
    inline constexpr xproperty<T, O>::xproperty(xp_owner_type* owner, const std::string& name, V&& value) noexcept(noexcept(std::is_nothrow_constructible<value_type>::value))
        : m_owner(owner), m_name(name), m_value(std::forward<V>(value))
    {
    }

    template <class T, class O>
    inline xproperty<T, O>::operator reference() noexcept
    {
        return m_value;
    }

    template <class T, class O>
    inline xproperty<T, O>::operator const_reference() const noexcept
    {
        return m_value;
    }

    template <class T, class O>
    inline auto xproperty<T, O>::operator()() noexcept -> reference
    {
        return m_value;
    }

    template <class T, class O>
    inline auto xproperty<T, O>::operator()() const noexcept -> const_reference
    {
        return m_value;
    }

    template <class T, class O>
    template <class Arg, class... Args>
    inline auto xproperty<T, O>::operator()(Arg&& arg, Args&&... args) && noexcept -> xp_owner_type
    {
        m_value = value_type(std::forward<Arg>(arg), std::forward<Args>(args)...);
        return std::move(*owner());
    }

    template <class T, class O>
    inline auto xproperty<T, O>::operator()(const value_type& arg) && noexcept -> xp_owner_type
    {
        m_value = arg;
        return std::move(*owner());
    }

    template <class T, class O>
    inline auto xproperty<T, O>::operator()(value_type&& arg) && noexcept -> xp_owner_type
    {
        m_value = std::move(arg);
        return std::move(*owner());
    }

    template <class T, class O>
    inline const std::string& xproperty<T, O>::name() const noexcept
    {
        return m_name;
    }

    template <class T, class O>
    template <class V>
    inline auto xproperty<T, O>::operator=(V&& value) -> reference
    {
        // lambda_validator(rhs);
        m_value = owner()->template invoke_validators<xproperty<T, O>>(m_name, std::forward<V>(value));
        owner()->notify(*this);
        owner()->invoke_observers(m_name);
        return m_value;
    }

    template <class T, class O>
    inline auto xproperty<T, O>::owner() noexcept -> xp_owner_type*
    {
        return m_owner;
    }
}

#endif
