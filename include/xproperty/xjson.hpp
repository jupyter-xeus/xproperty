/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XPROPERTY_JSON_HPP
#define XPROPERTY_JSON_HPP

#include "nlohmann/json.hpp" 

#include "xproperty_config.hpp"
#include "xproperty.hpp"

namespace xp
{
    /*************************************
     * to_json and from_json declaration *
     *************************************/

    template <class T, class O>
    void to_json(nlohmann::json&, const xproperty<T, O>&);

    template <class T, class O>
    void from_json(const nlohmann::json&, xproperty<T, O>&);

    /****************************************
     * to_json and from_json implementation *
     ****************************************/

    /**
     * @brief JSON serialization of xproperty.
     *
     * The to_json method is used by the nlohmann_json package for automatic
     * serialization of user-defined types. The method is picked up by
     * argument-dependent lookup.
     *
     * @param j a JSON object
     * @param e a const \ref xproperty
     */
    template <class T, class O>
    void to_json(nlohmann::json& j, const xproperty<T, O>& p)
    {
        using nlohmann::to_json;
        to_json(j, p());
    }

    /**
     * @brief JSON deserialization of an xproperty.
     *
     * The from_json method is used by the nlohmann_json library for automatic
     * serialization of user-defined types. The method is picked up by
     * argument-dependent lookup.
     *
     * @param j a const JSON object
     * @param e an \ref xproperty
     */
    template <class T, class O>
    void from_json(const nlohmann::json& j, xproperty<T, O>& p)
    {
        using nlohmann::from_json;
        from_json(j, p());
    }
}

#endif
