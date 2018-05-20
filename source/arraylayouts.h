/*
 *  arraylayouts.h
 *
 *  Copyright 2008 E. Onono <etuka@persistentnotions.co.uk>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
 
#pragma once

#include <initializer_list>
#include <array>
#include "array.h"
#include <iostream>

namespace marray {
  
  template<
    size_t M,
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
    typedef std::array<S, N> index_type;
    typedef trectlayoutref<N, N, S, D> layoutref_type;
    typedef trectlayoutref<N, N - 1, S, D> slice_layout;
    
    enum{ RANK = N };
    enum{ MAX_INDEX = N - 1 };
    
    trectlayout() : index_() {}
    trectlayout(const index_type& dimensions) : index_(calculate_index(dimensions)) {}
    trectlayout(const trectlayout& layout) : index_(layout.index_) {}
       
    const index_type& 
    index() const { return index_; }
    
    size_type
    dim(size_type i) const {
      assert(i < RANK);
      return (i < MAX_INDEX) ? index_[i] / index_[i + 1] : index_[MAX_INDEX];
    }
    
    /**
    footprint
    
    Number of elements in the array.
    */
    size_type
    footprint() const {
      return index_[0];
    }

    /**
    get_stride
    
    Calculate the data position implied by idx in the contiguous array block.  This is a 'stride'
    into the data from the data origin.
    */
    size_type 
    get_stride(const index_type& idx) const {
      size_type result(*(idx.end() - 1));         
      typename index_type::const_reverse_iterator ptr = idx.rbegin(), iptr = index_.rbegin();
      
      while(++ptr != idx.rend()) {
      result += (*ptr) * (*iptr);
      ++iptr;
      }
      return result;
    }
    
    slice_layout
    slice(size_type i) const {
      typename slice_layout::index_type idx;
      
      for(size_type j = 0;j < i;++j) {
      idx[j] = j;
      }
      
      for(size_type j = i + 1;j < RANK; ++j) {
      idx[j-1] = j;
      }
      
      return slice_layout(
          idx, 
          index_);
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

    index_type index_;
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
    typedef std::array<S, 2> index_type;
    typedef trectlayoutref<2, 2, S, D> layoutref_type;
    typedef trectlayoutref<2, 1, S, D> slice_layout;
    
    enum{ RANK = 2 };
    enum{ MAX_INDEX = 1 };
    
    trectlayout(const index_type& dimensions) : index_(calculate_index(dimensions)) {}
    trectlayout(const trectlayout& layout) : index_(layout.index_) {}
    trectlayout() : index_() {}
    
    size_type
    dim(size_type i) const {
      assert(i < RANK);
      return (i < MAX_INDEX) ? index_[i] / index_[i + 1] : index_[MAX_INDEX];
    }

    
    const index_type& 
    index() const { return index_; }
    
    /**
    footprint
    
    Number of elements in the array.
    */
    size_type
    footprint() const {
      return index_[0];
    }

    /**
    get_stride
    
    Calculate the data position implied by idx in the contiguous array block.  This is a 'stride'
    into the data from the data origin.
    */
    size_type 
    get_stride(const index_type& idx) const {
      size_type result(*(idx.end() - 1));           
      typename index_type::const_iterator ptr = idx.begin(), iptr = index_.begin();
      
      while(++ptr != idx.end()) {
        result += (*ptr) * (*iptr);
        ++iptr;
      }
      return result;
    }
    
    slice_layout
    slice(size_type i) const {
      typename slice_layout::mapping_index_type idx;
      
      for(size_type j = 0;j < i; ++j) {
        idx[j] = j;
      }
      
      for(size_type j = i + 1; j < RANK; ++j) {
        idx[j-1] = j;
      }
      
      return slice_layout(idx, index_);
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

    index_type index_;
  };
  
  /**
  trectlayoutref
  
  Encapsulates the shape of a weak array, referring requests for shape information to a parent
  layout.
  */
  template<
    size_t M,
    size_t N,
    typename S,
    typename D
 > struct trectlayoutref {
    enum{ TOP_RANK = M };
    enum{ RANK = N };
    enum{ MAX_INDEX = N - 1 };
    
    typedef S size_type;
    typedef D difference_type;
    typedef std::array<S, RANK> index_type;
    typedef std::array<S, TOP_RANK> mapped_index_type;
    typedef trectlayout<RANK, S, D> layout_type;
    typedef trectlayoutref<TOP_RANK, RANK - 1, S, D> slice_layout;

    trectlayoutref() : index_(), mapped_index_() {}
    
    trectlayoutref(const index_type& index, const std::array<S, TOP_RANK>& mapped_index) 
      : index_(index), mapped_index_(mapped_index){}
      
    size_type
    dim(size_type i) const {
      assert(i < mapped_index_.dim());
      return (index_[i] < mapped_index_.max_index()) ? 
            mapped_index_[index_[i] ] / mapped_index_[index_[i] + 1] : mapped_index_.back();
    }
    
    /**
    footprint
    
    Counterpart to the footprint function in trectlayoutref, but does not mean exactly the same
    thing.  Rather it is the index position of the end() pointer in the underlying contiguous array.
    */
    size_type
    footprint() const { return mapped_index_[ index_[0] ] ; }
    /**
    get_stride
    
    Calculate the data position implied by idx in the contiguous array block.  This is a 'stride'
    into the data from the data origin.
    */
    size_type
    get_stride(const index_type& idx) const {
      size_type result(0);
      typename index_type::const_iterator ptr = idx.begin(), iptr = index_.begin();
      
      while(ptr != idx.end()) {
        result += (*ptr) * (mapped_index_[*iptr]); //FIXME - need to increment some ptr here.
      }
      return result;
    }
    
    slice_layout
    slice(size_type i) const {
      typename slice_layout::mapping_index_type idx;
      
      for(size_type j = 0; j < i; ++j) {
        idx[j] = index_[j];
      }
      
      for(size_type j = i + 1; j < RANK; ++j) {
        idx[j - 1] = index_[j];
      }
      
      return slice_layout(idx, mapped_index_);
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
    index_type index_;
    mapped_index_type mapped_index_;
  };
  
  template<
    size_t M,
    typename S,
    typename D
 > struct trectlayoutref<M, 2, S, D> {
    enum{ TOP_RANK = M };
    enum{ RANK = 2 };
    enum{ MAX_INDEX = 1 };
    
    typedef S size_type;
    typedef D difference_type;
    typedef array<S, RANK> index_type;
    typedef array<S, TOP_RANK> mapped_index_type;
    typedef trectlayout<RANK, S, D> layout_type;
    typedef trectlayoutref<M, RANK - 1, S, D> slice_layout;

    trectlayoutref() : index_(), mapped_index_() {}
    
    trectlayoutref(const index_type& index, const mapped_index_type& mapped_index) 
      : index_(index), mapped_index_(mapped_index){}

    
    size_type
    dim(size_type i) const {
      assert(i < mapped_index_.size());
      return (index_[i] < mapped_index_.size() - 1) ? 
            mapped_index_[ index_[i] ] / mapped_index_[ index_[i] + 1 ] : mapped_index_.back();
    }

    /**
    footprint
    
    Counterpart to the footprint function in trectlayoutref, but does not mean exactly the same
    thing.  Rather it is the index position of the end() pointer in the underlying contiguous array.
    */
    size_type
    footprint() const { return mapped_index_[ index_[0] ] ; }
    
    /**
    get_stride
    
    Calculate the data position implied by idx in the contiguous array block.  This is a 'stride'
    into the data from the data origin.
    */
    size_type
    get_stride(const index_type& idx) const {
      size_type result(0);
      typename index_type::const_iterator ptr = idx.begin(), iptr = index_.begin();
      
      while(ptr != idx.end()) {
        result += (*ptr) * (mapped_index_[*iptr]);
      }
      return result;
    }
    
    slice_layout
    slice(size_type i) const {
      typename slice_layout::index_type idx;
      
      for(size_type j = 0; j < i; ++j) {
        idx[j] = j;
      }
      
      for(size_type j = i + 1;j < RANK; ++j) {
        idx[j-1] = j;
      }
      
      return slice_layout(idx, mapped_index_);
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
    index_type index_;
    mapped_index_type mapped_index_;
  };
}
