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

namespace z0rg {
namespace traits {

/**
 * @brief Trait used to indicate if the zero-copy version is being used
 * instead of the internal state of the zero_copy object.
 *
 * @tparam T A specialization of zero_copy or zero_copy_scoped.
 */
template <typename T>
struct is_zero_copy
{
    static const bool value = T::is_zero_copy;
};

}}
