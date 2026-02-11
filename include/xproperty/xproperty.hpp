/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XPROPERTY_HPP
#define XPROPERTY_HPP

#include <any>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace xp
{

    #define XP_NOEXCEPT(V) noexcept(noexcept((std::is_nothrow_constructible<V>::value)))

    /*************************
     * xproperty declaration *
     *************************/

    // Type, Owner Type

    template <class T, class O>
    class xproperty
    {
    public:

        using owner_type = O;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;

        // These constructors could constrain name to be a literal string
        // with `template <size_t N> const char (&name)[N]` but this increases
        // the binary size.
        xproperty(owner_type* owner, const char* name) XP_NOEXCEPT(value_type);
        template <class V>
        xproperty(owner_type* owner, const char* name, V&& value) XP_NOEXCEPT(value_type);
        template <class V, class LV>
        xproperty(owner_type* owner, const char* name, V&& value, LV&& lambda_validator) XP_NOEXCEPT(value_type);

        operator reference() noexcept;
        operator const_reference() const noexcept;

        reference operator()() noexcept;
        const_reference operator()() const noexcept;

        const char* name() const noexcept;

        template <class V>
        reference operator=(V&&);

    private:

        owner_type* owner() noexcept;

        std::ptrdiff_t m_offset;
        const char* m_name;
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
    //  - template <class T, class V>
    //    auto invoke_validators(const char* name, std::any& owner, V&& proposal);
    //  - void invoke_observers(const char* name, std::any& owner);
    //
    // The `T` typename is a universal reference on the proposed value.
    // The return type of `invoke_validator` must be convertible to the value_type of the property.

    #define XPROPERTY_GENERAL(T, O, D, DEFAULT_VALUE, lambda_validator)                                  \
    ::xp::xproperty<T, O> D = (::xp::xproperty<T, O>(static_cast<O*>(this), #D, T(DEFAULT_VALUE), lambda_validator));

    #define XPROPERTY_NODEFAULT(T, O, D)                                                                 \
    ::xp::xproperty<T, O> D = (::xp::xproperty<T, O>(static_cast<O*>(this), #D, T()));

    #define XPROPERTY_DEFAULT(T, O, D, V)                                                                \
    ::xp::xproperty<T, O> D = (::xp::xproperty<T, O>(static_cast<O*>(this), #D, T(V)));

    #define XPROPERTY_OVERLOAD(_1, _2, _3, _4, _5, NAME, ...) NAME

    #ifdef _MSC_VER
    // Workaround for MSVC not expanding macros
    #define XPROPERTY_EXPAND(x) x
    #define XPROPERTY(...) XPROPERTY_EXPAND(XPROPERTY_OVERLOAD(__VA_ARGS__, XPROPERTY_GENERAL, XPROPERTY_DEFAULT, XPROPERTY_NODEFAULT)(__VA_ARGS__))
    #else
    #define XPROPERTY(...) XPROPERTY_OVERLOAD(__VA_ARGS__, XPROPERTY_GENERAL, XPROPERTY_DEFAULT, XPROPERTY_NODEFAULT)(__VA_ARGS__)
    #endif

    /****************************
     * xproperty implementation *
     ****************************/

    template <class T, class O>
    inline xproperty<T, O>::xproperty(owner_type* owner,
                                      const char* name) XP_NOEXCEPT(value_type)
        : xproperty(owner, name, value_type())
    {
    }

    template <class T, class O>
    template <class V>
    inline xproperty<T, O>::xproperty(owner_type* owner,
                                      const char* name,
                                      V&& value) XP_NOEXCEPT(value_type)
        : m_offset(reinterpret_cast<char*>(this) - reinterpret_cast<char*>(owner))
        , m_name(name)
        , m_value(std::forward<V>(value))
    {
        owner->register_property(*this);
    }

    template <class T, class O>
    template <class V, class LV>
    inline xproperty<T, O>::xproperty(owner_type* owner,
                                      const char* name,
                                      V&& value,
                                      LV&& lambda_validator) XP_NOEXCEPT(value_type)
        : xproperty(owner, name, std::forward<V>(value))
    {
        owner->template validate<O, T>(m_name,
            [lambda_validator](O&, T& v)
            {
                lambda_validator(v);
            });
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
    inline const char* xproperty<T, O>::name() const noexcept
    {
        return m_name;
    }

    template <class T, class O>
    template <class V>
    inline auto xproperty<T, O>::operator=(V&& value) -> reference
    {
        auto owner_any = std::any(std::ref(*owner()));
        m_value = owner()->template invoke_validators<T>(m_name, owner_any, std::forward<V>(value));
        owner()->notify(m_name, m_value);
        owner()->invoke_observers(m_name, owner_any);
        return m_value;
    }

    template <class T, class O>
    inline auto xproperty<T, O>::owner() noexcept -> owner_type*
    {
        return reinterpret_cast<owner_type*>(
            reinterpret_cast<char*>(this) - m_offset
        );
    }
}

#endif
