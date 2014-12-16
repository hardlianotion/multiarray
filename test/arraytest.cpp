/*
 *      arraytest.cpp
 *
 *      Copyright 2008 E. Onono <etuka@persistentnotions.co.uk>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <array.h>
#include <vector>
#include <gtest/gtest.h>

using namespace marray;
using namespace std;
typedef tarray<double> d_array;
typedef tarray<double, double*, true, size_t> wd_array;


TEST(arraytest,testCreateArray) {
    d_array array_(10);
    wd_array warray_(array_);
    
    EXPECT_EQ(warray_.dim(), 10);
    EXPECT_EQ(array_.dim(), 10);

    wd_array::iterator wptr = warray_.begin();
    
    for(
        d_array::iterator ptr = array_.begin();
        ptr != array_.end();
        ++ptr, ++wptr
     ) {
        EXPECT_EQ(*ptr, *wptr);
    }
    
}

TEST(arraytest,testArrayIndexing) {
    d_array array_(3);
    wd_array warray_(array_);
    
    for(
        size_t i = 0;
        i <array_.dim();
        ++i
    ) {
        array_[i] = i;
    }
    
    for(
        size_t i = 0; 
        i <array_.dim(); 
        ++i
    ) {
        EXPECT_EQ(array_[i], i);
        EXPECT_EQ(warray_[i], i);
    }
    
    EXPECT_EQ(array_.front(), 0);
    EXPECT_EQ(array_.back(), 2);
    EXPECT_EQ(warray_.front(), 0);
    EXPECT_EQ(warray_.back(), 2);
}

TEST(arraytest,testIterator) {
    d_array array_(10);
    wd_array warray_(array_);
    double x(0.0);
    
    for(
        d_array::iterator ptr = array_.begin();
        ptr != array_.end();
        ++ptr
    ) {
        *ptr = x++;
    }
    
    x = 0.0;
    
    wd_array::const_iterator wcptr = warray_.begin();
    d_array::const_iterator cptr = array_.begin();
    
    for(
        wd_array::iterator ptr = warray_.begin();
        ptr != warray_.end();
        ++ptr, ++wcptr, ++cptr
    ) {
        EXPECT_EQ(*ptr, x++);
        EXPECT_EQ(*cptr, *ptr);
        EXPECT_EQ(*wcptr, *ptr);
    }
}

