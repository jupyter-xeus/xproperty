/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <cstddef>

namespace xp
{

    inline std::size_t& get_observe_count()
    {
        static std::size_t count = 0;
        return count;
    }

    inline std::size_t& get_validate_count()
    {
        static std::size_t count = 0;
        return count;
    }

    inline void reset_counter()
    {
        get_observe_count() = 0;
        get_validate_count() = 0;
    }
}

#endif

