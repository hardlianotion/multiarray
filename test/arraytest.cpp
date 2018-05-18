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
#define CATCH_CONFIG_MAIN 
#include <array.h>
#include <vector>
#include <catch/catch.hpp>

using namespace marray;
using namespace std;
typedef tarray<double> d_array;
typedef tarray<double, double*, true, size_t> wd_array;


TEST_CASE("Test weak arrays refer to underlying array", "[array]") {
    d_array array_(10);
    wd_array warray_(array_);
    
    REQUIRE(warray_.dim() == 10);
    REQUIRE(array_.dim() == 10);

    wd_array::iterator wptr = warray_.begin();
    
    for(
        d_array::iterator ptr = array_.begin();
        ptr != array_.end();
        ++ptr, ++wptr
     ) {
        REQUIRE(*ptr == *wptr);
    }
    
}

TEST_CASE("Test weak indexing is consistent with array","[array]") {
    d_array array_(3);
    wd_array warray_(array_);
    
    for(
        size_t i = 0;
        i < array_.dim();
        ++i
    ) {
        array_[i] = i;
    }
    
    for(
        size_t i = 0; 
        i < array_.dim(); 
        ++i
    ) {
        REQUIRE(array_[i] == i);
        REQUIRE(warray_[i] == i);
    }
    
    REQUIRE(array_.front() == 0);
    REQUIRE(array_.back() == 2);
    REQUIRE(warray_.front() == 0);
    REQUIRE(warray_.back() == 2);
}

TEST_CASE("Iterators for weak arrays and arrays work the same way", "[array]") {
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
        REQUIRE(*ptr == x++);
        REQUIRE(*cptr == *ptr);
        REQUIRE(*wcptr == *ptr);
    }
}

