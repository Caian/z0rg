/*
 * Copyright (C) 2018 Caian Benedicto <caianbene@gmail.com>
 *
 * This file is part of s1o.
 *
 * s1o is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * s1o is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Make; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "z0rg/zero_copy.hpp"
#include "z0rg/zero_copy_scoped.hpp"
#include "z0rg/traits/is_zero_copy.hpp"

#include <gtest/gtest.h>
#include <iterator>
#include <vector>

namespace {

void do_work(int i, int& j)
{
    j = i;
}

/**
 *
 */
TEST(VectorZeroCopy, Begin1)
{
    const int N = 50;

    typedef std::vector<int> Container;
    typedef Container::iterator It;
    typedef z0rg::zero_copy<int, It> ZC;

    bool is_zero_copy = z0rg::traits::is_zero_copy<ZC>::value;
    ASSERT_EQ(true, is_zero_copy);

    Container result(N);
    It it = result.begin();

    for (int i = 0; i < N; i++, it++) {

        ZC zc;

        do_work(i, zc.get(it));
        zc.set(it);
    }

    ASSERT_EQ(N, result.size());

    Container::iterator j = result.begin();
    for (int i = 0; i < N; i++, j++) {
        ASSERT_EQ(i, *j);
    }
}

/**
 *
 */
TEST(VectorZeroCopy, Begin2)
{
    const int N = 50;

    typedef std::vector<int> Container;
    typedef Container::iterator It;
    typedef z0rg::zero_copy<int, It> ZC;

    bool is_zero_copy = z0rg::traits::is_zero_copy<ZC>::value;
    ASSERT_EQ(true, is_zero_copy);

    Container result(N);
    It it = result.begin();

    ZC zc;

    for (int i = 0; i < N; i++, it++) {
        do_work(i, zc.get(it));
        zc.set(it);
    }

    ASSERT_EQ(N, result.size());

    Container::iterator j = result.begin();
    for (int i = 0; i < N; i++, j++) {
        ASSERT_EQ(i, *j);
    }
}

/**
 *
 */
TEST(VectorZeroCopyScoped, Begin)
{
    const int N = 50;

    typedef std::vector<int> Container;
    typedef Container::iterator It;
    typedef z0rg::zero_copy_scoped<int, It> ZC;

    bool is_zero_copy = z0rg::traits::is_zero_copy<ZC>::value;
    ASSERT_EQ(true, is_zero_copy);

    Container result(N);
    It it = result.begin();

    for (int i = 0; i < N; i++, it++) {
        ZC zc(it);
        do_work(i, zc.get());
    }

    ASSERT_EQ(N, result.size());

    Container::iterator j = result.begin();
    for (int i = 0; i < N; i++, j++) {
        ASSERT_EQ(i, *j);
    }
}

}
