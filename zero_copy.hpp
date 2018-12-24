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
 * along with GNU Make; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#pragma once

#include <iterator>

namespace z0rg {

namespace impl {

/**
 * @brief Generic implementation of the zero copy object used when the
 * iterator does not reference data directly (the reference type does not
 * match the object type), so an internal state with the object must be kept.
 *
 * @tparam T The type of the object being referenced.
 * @tparam R The reference type of the iterator.
 */
template<typename T, typename R>
class zero_copy_impl
{
public:

    static const bool is_zero_copy = false;

private:

    T _t; /** The object that will be referenced instead of the iterator. */

public:

    /**
     * @brief Return the internal state of the zero_copy_impl object instead
     * of dereferencing the iterator.
     *
     * @tparam IT The type of the iterator being accessed.
     *
     * @param it The iterator being accessed.
     *
     * @return T& A reference to the internal state of the zero_copy_impl
     * object.
     */
    template <typename IT>
    inline T& get(IT it)
    {
        (void)it;

        return _t;
    }

    /**
     * @brief Assign the internal state to the iterator.
     *
     * @tparam IT The type of the iterator being set.
     *
     * @param it The iterator being set.
     */
    template <typename IT>
    inline void set(IT it) const
    {
        *it = _t;
    }
};

/**
 * @brief Specialized implementation of the zero copy object used when the
 * iterator references data directly (the reference type matches the object
 * type), so a reference to the object pointed by the iterator is returned
 * directly.
 *
 * @tparam T The type of the object being referenced.
 */
template<typename T>
class zero_copy_impl<T, T&>
{
public:

    static const bool is_zero_copy = true;

    /**
     * @brief Return a reference to the object pointed by the iterator.
     *
     * @tparam IT The type of the iterator being accessed.
     *
     * @param it The iterator being accessed.
     *
     * @return T& A reference to the object pointed by the iterator.
     */
    template <typename IT>
    inline T& get(IT it) const
    {
        return *it;
    }

    /**
     * @brief Don't do anything because the reference of iterator was
     * used directly.
     *
     * @tparam IT The type of the iterator being set.
     *
     * @param it The iterator being set.
     */
    template <typename IT>
    inline void set(IT it) const
    {
        (void)it;
    }
};

}

/**
 * @brief Class to enable zero copy when dealing with iterators that allows
 * getting a reference to the object it points to, otherwise zero_copy will
 * use an internal state and then set the iterator value.
 *
 * @tparam T The type of the object being referenced.
 * @tparam IT The type of the iterator used to store the result.
 */
template<typename T, typename IT>
class zero_copy
{
public:

    typedef typename std::iterator_traits<IT>::reference R;
    typedef impl::zero_copy_impl<T, R> Impl;

    static const bool is_zero_copy = Impl::is_zero_copy;

private:

    Impl _impl;

public:

    /**
     * @brief Get a reference to the object, that is either pointed by the
     * iterator (if it allows), or to an internal state of the zero_copy
     * object.
     *
     * @param it The iterator being accessed.
     *
     * @return T& A reference to the object, that is either pointed by the
     * iterator (if it allows), or to an internal state of the zero_copy
     * object.
     */
    inline T& get(IT it)
    {
        return _impl.get(it);
    }

    /**
     * @brief Assign the internal state of the zero_copy object to the
     * iterator (if it does not expose the reference to the pointed object
     * directly). Otherwise do nothing.
     *
     * @param it The iterator being set.
     */
    inline void set(IT it)
    {
        _impl.set(it);
    }
};

}
