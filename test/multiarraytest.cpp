/*
 *      multiarraytest.cpp
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

#include <multiarray.h>
#include <cmath>
#include <iostream>
#include <gtest/gtest.h>

using namespace marray;
using namespace std;

typedef tmultiarray<double, 3> dm_array3;
typedef tmultiarray<double, 2> dm_array2;
typedef tarray<double> dm_array;


TEST(multiarraytest,test_create_multiarray) {
    array<size_t, 3> index3;
    array<size_t, 2> index2;
    index3[0] = 2; index3[1] = 3; index3[2] = 4; 
    index2[0] = 2; index2[1] = 3;
    
    trectlayout<3> layout3(index3);
    trectlayout<2> layout2(index2);
    
    dm_array3 array_3(layout3);
    dm_array2 array_2(layout2);
            
    EXPECT_EQ(layout3.dim(0), 2);
    EXPECT_EQ(layout3.dim(1), 3);
    EXPECT_EQ(layout3.dim(2), 4);
    
    EXPECT_EQ(layout2.dim(0), 2);
    EXPECT_EQ(layout2.dim(1), 3);
}

TEST(multiarraytest,test_indexer) {
    array<size_t, 3> index3;
    array<size_t, 2> index2;
    index3[0] = 2, index3[1] = 3, index3[2] = 4;
    
    trectlayout<3> layout3(index3);
    
    trectlayout<3>::slice_layout layout3_slice = layout3.slice(1);
            
    EXPECT_EQ(layout3.dim(0), 2);
    EXPECT_EQ(layout3.dim(1), 3);
    EXPECT_EQ(layout3.dim(2), 4);

    EXPECT_EQ(layout3_slice.dim(0), 2);
    EXPECT_EQ(layout3_slice.dim(1), 4);
    trectlayout<2> layout2(index2);
}

TEST(multiarraytest,test_parallel_indexing) {
    array<size_t, 3> index3;
    array<size_t, 2> index2;
    index3[0] = 2; index3[1] = 3; index3[2] = 4; 
    index2[0] = 2; index2[1] = 3;
    
    trectlayout<3> layout3(index3);
    trectlayout<2> layout2(index2);
    
    dm_array3 array_3(layout3);
    dm_array2 array_2(layout2);
    double data = 0.0;
    for(dm_array3::iterator ptr = array_3.begin(); ptr != array_3.end(); ++ptr) {
        *ptr = data++;
    }
    data = 0.0;
    for(size_t i = 0; i <3; ++i) index3[i] = 0;
    
    for(size_t i = 0; i <2; ++i) {
        for(size_t j = 0; j <3; ++j) {
            for(size_t k = 0; k <4; ++k) {
                cout << "array_3[i][j][k] = " << array_3[i][j][k] << " array_3(index3) = " << array_3(index3) << endl;
                EXPECT_EQ(array_3(index3), data);        
                EXPECT_EQ(array_3[i][j][k], data++);
                
                ++index3[2];
            }
            ++index3[1]; index3[2] = 0;
        }
        ++index3[0]; index3[1] = 0; index3[2] = 0;

        cout << endl << endl;
    }

}
