/*
 * Copyright (C) 2018 Caian Benedicto <caianbene@gmail.com>
 *
 * This file is part of z0rg.
 *
 * z0rg is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * z0rg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with z0rg.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "zero_copy.hpp"

namespace z0rg {

/**
 * @brief Scoped RAII class to enable zero copy when dealing with iterators
 * that allows getting a reference to the object it points to, otherwise
 * zero_copy will use an internal state and then set the iterator value.
 *
 * @tparam T The type of the object being referenced.
 * @tparam IT The type of the iterator used to store the result.
 */
template<typename T, typename IT>
class zero_copy_scoped
{
public:

    typedef zero_copy<T, IT> Impl;

    static const bool is_zero_copy = Impl::is_zero_copy;

private:

    Impl _impl;
    IT _it;

public:

    /**
     * @brief Construct a new zero_copy_scoped object with an associated
     * iterator.
     *
     * @param it A reference to the iterator bounded to the scope of the
     * class.
     */
    zero_copy_scoped(
        IT& it
    ) :
        _impl(),
        _it(it)
    {
    }

    /**
     * @brief Destroy the object and assign the internal state of the
     * zero_copy object to the iterator (if it does not expose the reference
     * to the pointed object directly).
     *
     */
    ~zero_copy_scoped()
    {
        _impl.set(_it);
    }

    /**
     * @brief Get a reference to the object, that is either pointed by the
     * iterator (if it allows), or to an internal state of the zero_copy
     * object.
     *
     * @return T& A reference to the object, that is either pointed by the
     * iterator (if it allows), or to an internal state of the zero_copy
     * object.
     */
    inline T& get()
    {
        return _impl.get(_it);
    }
};

}
