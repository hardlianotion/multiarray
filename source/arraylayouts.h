/*
 *      arraylayouts.h
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
 
#pragma once

#include <array>
#include "array.h"
#include <iostream>

namespace marray {
    
    using std::array;
    using namespace std;
    
    template<
        size_t N,
        typename S = size_t,
        typename D = ptrdiff_t
   > struct trectlayoutref;
    
    /**
    trectlayout
    
    Encapsulates the shape of a multidimensional array.  Used to determine the position of a data point,
    given a referring index.
    */
    
    template<
        size_t N,
        typename S = size_t,
        typename D = ptrdiff_t
   > struct trectlayout {
        
        typedef S size_type;
        typedef D difference_type;
        typedef array<S, N> index_type;
        typedef trectlayoutref<N, S, D> layoutref_type;
        typedef trectlayoutref<N - 1, S, D> slice_layout;
        
        enum{ RANK = N };
        enum{ MAX_INDEX = N - 1 };
        
        trectlayout() : _index() {}
        trectlayout(const index_type& dimensions) : _index(calculate_index(dimensions)) {}
        trectlayout(const trectlayout& layout) : _index(layout._index) {}
        
        const index_type& 
        index() const { return _index; }
        
        size_type
        dim(size_type i) const {
            assert(i <RANK);
            return (i <MAX_INDEX) ? _index[i] / _index[i + 1] : _index[MAX_INDEX];
        }
        
        /**
        footprint
        
        Number of elements in the array.
        */
        size_type
        footprint() const {
            return _index[0];
        }

        /**
        get_stride
        
        Calculate the data position implied by idx in the contiguous array block.  This is a 'stride'
        into the data from the data origin.
        */
        size_type 
        get_stride(const index_type& idx) const {
            size_type result(*(idx.end() - 1));             
            typename index_type::const_reverse_iterator ptr = idx.rbegin(), iptr = _index.rbegin();
            
            while(++ptr != idx.rend()) {
                result += (*ptr) * (*iptr);
                ++iptr;
            }
            return result;
        }
        
        slice_layout
        slice(size_type i) const {
            typename slice_layout::mapping_index_type idx;
            
            for(
                size_type j = 0;
                j <i;
                ++j
           ) {
                idx[j] = j;
            }
            
            for(
                size_type j = i + 1;
                j <RANK;
                ++j
           ) {
                idx[j-1] = j;
            }
            
            
            return slice_layout(
                        idx, 
                        typename slice_layout::mapped_index_type(
                            const_cast<typename index_type::iterator>(_index.begin()), RANK 
                       ) 
                   );
        }
        
    private:
        /**
        calculate_index
        Calculate the index strides from the dimensions of the hypercube.  These double as iterator limits.
        */
            index_type
        calculate_index(const index_type& dimensions) {
            index_type result(dimensions);
            
            typename index_type::const_reverse_iterator dimptr = dimensions.rbegin();
            typename index_type::reverse_iterator idxptr = result.rbegin();
            ++dimptr;
            
            while(dimptr != dimensions.rend()) {
                *(idxptr + 1) *= (*idxptr);
                ++dimptr, ++idxptr; 
            }
            cout <<endl;
            return result;
        }

        index_type _index;
    };
    
    /**
    trectlayout
    
    Encapsulates the shape of a multidimensional array.  Used to determine the position of a data point,
    given a referring index.
    */
    
    template<
        typename S,
        typename D
   > struct trectlayout<2, S, D> {
        
        typedef S size_type;
        typedef D difference_type;
        typedef array<S, 2> index_type;
        typedef trectlayoutref<2, S, D> layoutref_type;
        typedef trectlayoutref<1, S, D> slice_layout;
        
        enum{ RANK = 2 };
        enum{ MAX_INDEX = 1 };
        
        trectlayout(const index_type& dimensions) : _index(calculate_index(dimensions)) {}
        trectlayout(const trectlayout& layout) : _index(layout._index) {}
        trectlayout() : _index() {}
        
        size_type
        dim(size_type i) const {
            assert(i <RANK);
            return (i <MAX_INDEX) ? _index[i] / _index[i + 1] : _index[MAX_INDEX];
        }

        
        const index_type& 
        index() const { return _index; }
        
        /**
        footprint
        
        Number of elements in the array.
        */
        size_type
        footprint() const {
            return _index[0];
        }

        /**
        get_stride
        
        Calculate the data position implied by idx in the contiguous array block.  This is a 'stride'
        into the data from the data origin.
        */
        size_type 
        get_stride(const index_type& idx) const {
            size_type result(*(idx.end() - 1));             
            typename index_type::const_iterator ptr = idx.begin(), iptr = _index.begin();
            
            while(++ptr != idx.end()) {
                result += (*ptr) * (*iptr);
                ++iptr;
            }
            return result;
        }
        
        slice_layout
        slice(size_type i) const {
            typename slice_layout::mapping_index_type idx;
            
            for(
                size_type j = 0;
                j <i;
                ++j
           ) {
                idx[j] = j;
            }
            
            for(
                size_type j = i + 1;
                j <RANK;
                ++j
           ) {
                idx[j-1] = j;
            }
            
            return slice_layout(idx, typename slice_layout::mapped_index_type(_index.begin(), index_type::RANK));
        }
        
    private:
        /**
        calculate_index
        Calculate the index strides from the dimensions of the hypercube.  These double as iterator limits.
        */
            index_type
        calculate_index(const index_type& dimensions) {
            index_type result(dimensions);
            
            typename index_type::const_reverse_iterator dimptr = dimensions.rbegin(); 
            typename index_type::reverse_iterator idxptr = result.rbegin();
            ++dimptr;
            while(dimptr != dimensions.rend()) {
                *(idxptr + 1) *= (*idxptr);
                ++dimptr, ++idxptr; 
            }
            return result;
        }

        index_type _index;
    };
    
    /**
    trectlayoutref
    
    Encapsulates the shape of a weak array, referring requests for shape information to a parent
    layout.
    */
    template<
        size_t N,
        typename S,
        typename D
   > struct trectlayoutref {
        typedef S size_type;
        typedef D difference_type;
        typedef array<S, N> index_type;
        typedef array<size_t, N> mapping_index_type;
        typedef tarray<S, S*, true, size_t, ptrdiff_t> mapped_index_type;
        typedef trectlayout<N, S, D> layout_type;
        typedef trectlayoutref<N - 1, S, D> slice_layout;

        
        enum{ RANK = N };
        enum{ MAX_INDEX = N - 1 };
        
        trectlayoutref() : _index(), _mapped_index() {}
        
        trectlayoutref(const index_type& index, const mapped_index_type& mapped_index) 
            : _index(index), _mapped_index(mapped_index){}
            
        size_type
        dim(size_type i) const {
            assert(i <_mapped_index.dim());
            return (_index[i] <_mapped_index.max_index()) ? 
                    _mapped_index[_index[i] ] / _mapped_index[_index[i] + 1] : _mapped_index.last();
        }
        
        /**
        footprint
        
        Counterpart to the footprint function in trectlayoutref, but does not mean exactly the same
        thing.  Rather it is the index position of the end() pointer in the underlying contiguous array.
        */
        size_type
        footprint() const { return _mapped_index[ _index[0] ] ; }
        /**
        get_stride
        
        Calculate the data position implied by idx in the contiguous array block.  This is a 'stride'
        into the data from the data origin.
        */
        size_type
        get_stride(const index_type& idx) const {
            size_type result(0);
            typename index_type::const_iterator ptr = idx.begin(), iptr = _index.begin();
            
            while(ptr != idx.end()) {
                result += (*ptr) * (_mapped_index[*iptr]);
            }
            return result;
        }
        
        slice_layout
        slice(size_type i) const {
            typename slice_layout::mapping_index_type idx;
            
            for(
                size_type j = 0;
                j <i;
                ++j
           ) {
                idx[j] = j;
            }
            
            for(
                size_type j = i + 1;
                j <RANK;
                ++j
           ) {
                idx[j-1] = j;
            }
            
            return slice_layout(idx, _mapped_index);
        }
        
        /**
        layout
        
        returns an equivalent rectangular layout.  This would be used when copying a weak
        array into a non-weak (data-owning) array.
        */
        layout_type
        layout() const {
            layout_type result;
            /*
            TODO - calculate the correct fixed strides given reference strides.
            */
            return result;
        }
        
    private:
        mapping_index_type _index;
        mapped_index_type _mapped_index;
    };
    
    template<
        typename S,
        typename D
   > struct trectlayoutref<2, S, D> {
        typedef S size_type;
        typedef D difference_type;
        typedef array<S, 2> index_type;
        typedef array<size_t, 2> mapping_index_type;
        typedef tarray<S, S*, true, size_t, ptrdiff_t> mapped_index_type;
        typedef trectlayout<2, S, D> layout_type;
        typedef trectlayoutref<1, S, D> slice_layout;

        
        enum{ RANK = 2 };
        enum{ MAX_INDEX = 1 };
        
        trectlayoutref() : _index(), _mapped_index() {}
        
        trectlayoutref(const index_type& index, const mapped_index_type& mapped_index) 
            : _index(index), _mapped_index(mapped_index){}

        
        size_type
        dim(size_type i) const {
            assert(i <_mapped_index.dim());
            return (_index[i] <_mapped_index.max_index()) ? 
                    _mapped_index[ _index[i] ] / _mapped_index[ _index[i] + 1 ] : _mapped_index.last();
        }

        /**
        footprint
        
        Counterpart to the footprint function in trectlayoutref, but does not mean exactly the same
        thing.  Rather it is the index position of the end() pointer in the underlying contiguous array.
        */
        size_type
        footprint() const { return _mapped_index[ _index[0] ] ; }
        
        /**
        get_stride
        
        Calculate the data position implied by idx in the contiguous array block.  This is a 'stride'
        into the data from the data origin.
        */
        size_type
        get_stride(const index_type& idx) const {
            size_type result(0);
            typename index_type::const_iterator ptr = idx.begin(), iptr = _index.begin();
            
            while(ptr != idx.end()) {
                result += (*ptr) * (_mapped_index[*iptr]);
            }
            return result;
        }
        
        slice_layout
        slice(size_type i) const {
            typename slice_layout::mapping_index_type idx;
            
            for(
                size_type j = 0;
                j <i;
                ++j
           ) {
                idx[j] = j;
            }
            
            for(
                size_type j = i + 1;
                j <RANK;
                ++j
           ) {
                idx[j-1] = j;
            }
            
            return slice_layout(idx, _mapped_index);
        }
        
        /**
        layout
        
        returns an equivalent rectangular layout.  This would be used when copying a weak
        array into a non-weak (data-owning) array.
        */
        layout_type
        layout() const {
            layout_type result;
            /*
            TODO - calculate the correct fixed strides given reference strides.
            */
            return result;
        }
        
    private:
        mapping_index_type _index;
        mapped_index_type _mapped_index;
    };
}
