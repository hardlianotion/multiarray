/*
 *      iteratortest.cpp
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
#include <catch/catch.hpp>

using namespace marray;
using namespace std;
typedef tarray<double> d_array;
typedef tarray<double, double*, true, size_t> wd_array;


TEST_CASE("iterators increment works like ptr", "[marray]") {
    d_array array_(10);
    array_[9] = 10; 
    
    size_t i = 0; 
    for(
        d_array::iterator ptr = array_.begin();
        ptr != array_.end();
        ++ptr, ++i
     ) {
        REQUIRE(*ptr == array_[i]);
    }
    REQUIRE(10 == array_[9]);
    
}

TEST_CASE("Can initialise const iterator with an iterator", "[marray]") {
    d_array array_(10);
    array_[9] = 10;
   
    d_array::iterator ptr = array_.begin();
    tconst<d_array::iterator> cptr(ptr);

    REQUIRE(array_.begin() == cptr);

    while(ptr != array_.end()) {
        REQUIRE(ptr != array_.end());
        REQUIRE(*ptr == *cptr);
        
        d_array::iterator nptr = ptr++;
        REQUIRE(1 == ptr - nptr);
        tconst<d_array::iterator> ncptr = cptr++;
        REQUIRE(1 == cptr - ncptr);
    }
    REQUIRE(10 == *(--cptr));
}

