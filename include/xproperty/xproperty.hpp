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

namespace xp
{

    /****************************
     * xoffsetof implementation *
     ****************************/

    #ifdef __clang__
        #define xoffsetof(O, M)                                    \
        _Pragma("clang diagnostic push")                           \
        _Pragma("clang diagnostic ignored \"-Winvalid-offsetof\"") \
        offsetof(O, M)                                             \
        _Pragma("clang diagnostic pop")
    #else
        #define xoffsetof(O, M) offsetof(O, M)
    #endif

    /*************************
     * xproperty declaration *
     *************************/

    // Type, Owner Type, Derived Type

    template <class T, class O, class D>
    class xproperty
    {
    public:

        using owner_type = O;
        using derived_type = D;

        derived_type& derived_cast() & noexcept;
        const derived_type& derived_cast() const & noexcept;
        derived_type derived_cast() && noexcept;

        using value_type = T;
        using reference = T&;
        using const_reference = const T&;

        constexpr xproperty() noexcept(noexcept(std::is_nothrow_constructible<value_type>::value));
        constexpr xproperty(const_reference value) noexcept(noexcept(std::is_nothrow_constructible<value_type>::value));
        constexpr xproperty(value_type&& value) noexcept(noexcept(std::is_nothrow_move_constructible<value_type>::value));

        operator reference() noexcept;
        operator const_reference() const noexcept;

        reference operator()() noexcept;
        const_reference operator()() const noexcept;

        template <class V>
        reference operator=(V&& value);

    private:

        owner_type* owner() noexcept;

        value_type m_value;
    };

    /*******************
     * XPROPERTY macro *
     *******************/

    // XPROPERTY(Type, Owner, Name)
    //
    // Defines a property of the specified type and name, for the specified owner type.
    //
    // The owner type must have two template methods
    //
    //  - invoke_validators<std::size_t Offset, typename const_ref>(const_ref value);
    //  - invoke_observers<std::size_t Offset>();
    //
    // The `Offset` integral parameter is the offset of the observed member in the owner class.
    // The `const_ref` typename is a constant reference type on the proposed value.

    #define XPROPERTY(T, O, D)                                                              \
    class D##_property : public ::xp::xproperty<T, O, D##_property>                         \
    {                                                                                       \
    public:                                                                                 \
                                                                                            \
        template <class V>                                                                  \
        inline typename ::xp::xproperty<T, O, D##_property>::reference operator=(V&& value) \
        {                                                                                   \
            return ::xp::xproperty<T, O, D##_property>::operator=(std::forward<V>(value));  \
        }                                                                                   \
                                                                                            \
        static inline const std::string& name() noexcept                                    \
        {                                                                                   \
            static const std::string name = #D;                                             \
            return name;                                                                    \
        }                                                                                   \
                                                                                            \
        static inline constexpr std::size_t offset() noexcept                               \
        {                                                                                   \
            return xoffsetof(O, D);                                                         \
        }                                                                                   \
    } D;

    /***********************
     * MAKE_OBSERVED macro *
     ***********************/

    // MAKE_OBSERVED()
    //
    // Adds the required boilerplate for an obsered structure.

    #define MAKE_OBSERVED()                                                                 \
    template <class P>                                                                      \
    inline void notify(const P&) const {}                                                   \
    template <class P>                                                                      \
    inline void invoke_observers() const {}                                                 \
    template <class P, class V>                                                             \
    inline auto invoke_validators(V&& r) const { return r; }

    /*************************
     * XOBSERVE_STATIC macro *
     *************************/

    // XOBSERVE_STATIC(Type, Owner, Name)
    //
    // Set up the static notifier for the specified property

    #define XOBSERVE_STATIC(T, O, D) \
    template <>                      \
    inline void O::invoke_observers<O::D##_property>() const

    /**************************
     * XVALIDATE_STATIC macro *
     **************************/

    // XVALIDATE_STATIC(Type, Owner, Name, Proposal Argument Name)
    //
    // Set up the static validator for the specified property

    #define XVALIDATE_STATIC(T, O, D, A) \
    template <>                          \
    inline auto O::invoke_validators<O::D##_property, T>(T && A) const

    /****************************
     * xproperty implementation *
     ****************************/

    template <class T, class O, class D>
    inline auto xproperty<T, O, D>::derived_cast() & noexcept -> derived_type&
    {
        return *static_cast<derived_type*>(this);
    }

    template <class T, class O, class D>
    inline auto xproperty<T, O, D>::derived_cast() const & noexcept -> const derived_type&
    {
        return *static_cast<const derived_type*>(this);
    }

    template <class T, class O, class D>
    inline auto xproperty<T, O, D>::derived_cast() && noexcept -> derived_type
    {
        return *static_cast<derived_type*>(this);
    }

    template <class T, class O, class D>
    inline constexpr xproperty<T, O, D>::xproperty() noexcept(noexcept(std::is_nothrow_constructible<value_type>::value))
        : m_value()
    {
    }

    template <class T, class O, class D>
    inline constexpr xproperty<T, O, D>::xproperty(value_type&& value) noexcept(noexcept(std::is_nothrow_move_constructible<value_type>::value))
        : m_value(value)
    {
    }

    template <class T, class O, class D>
    inline constexpr xproperty<T, O, D>::xproperty(const_reference value) noexcept(noexcept(std::is_nothrow_constructible<value_type>::value))
        : m_value(value)
    {
    }

    template <class T, class O, class D>
    inline xproperty<T, O, D>::operator reference() noexcept
    {
        return m_value;
    }

    template <class T, class O, class D>
    inline xproperty<T, O, D>::operator const_reference() const noexcept
    {
        return m_value;
    }

    template <class T, class O, class D>
    inline auto xproperty<T, O, D>::operator()() noexcept -> reference
    {
        return m_value;
    }

    template <class T, class O, class D>
    inline auto xproperty<T, O, D>::operator()() const noexcept -> const_reference
    {
        return m_value;
    }

    template <class T, class O, class D>
    template <class V>
    inline auto xproperty<T, O, D>::operator=(V&& value) -> reference
    {
        m_value = owner()->template invoke_validators<derived_type>(std::forward<V>(value));
        owner()->notify(derived_cast());
        owner()->template invoke_observers<derived_type>();
        return m_value;
    }

    template <class T, class O, class D>
    inline auto xproperty<T, O, D>::owner() noexcept -> owner_type*
    {
        return reinterpret_cast<owner_type*>(reinterpret_cast<char*>(this) - derived_type::offset());
    }
}

#endif
