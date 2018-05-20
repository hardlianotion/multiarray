/*
 *    array.h
 *
 *    Copyright 2008 E. Onono <etuka@persistentnotions.co.uk>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#pragma once
#include "arrayiterator.h"

namespace marray {
  
  template<
    typename T,
    typename PT = T*,
    typename S = size_t,
    typename D = ptrdiff_t
  > struct tindexeddata {
  
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef S size_type;
    typedef D difference_type;
    typedef titerator<T, PT, S, D>  iterator;
    typedef tconst<iterator> const_iterator;
    
    reference
    operator[](size_type i) { return *(begin_ + i); }
    
    const_reference
    operator[](size_type i) const { return *(begin_ + i); }
    
    const_iterator
    begin() const {return const_iterator(begin_);  }
    
    iterator
    begin() { return begin_; }
    
    const_iterator
    end() const { return const_iterator(end_);  }
    
    iterator
    end() { return end_; }
    
    size_type
    dim() const {
      return static_cast<size_type>(strides(begin(), end()));
    }
    
    size_type
    max_index() const { return dim() - 1; }
    
    const_reference
    front() const { return *this->begin(); }
    
    reference
    front() { return *this->begin(); }
    
    const_reference
    back() const { return *(this->end() - 1); }
    
    reference
    back() { return *(this->end() - 1); }
    
    void
    reset(iterator begin, iterator end) {
      begin_ = begin;
      end_ = end;
    }
    
  protected:
    tindexeddata() : begin_(nullptr), end_(nullptr) {}
    tindexeddata(const tindexeddata& rhs) : begin_(rhs.begin_), end_(rhs.end_) {}
    tindexeddata(iterator begin, iterator end) : begin_(begin), end_(end) {}
    tindexeddata(iterator begin, size_type dim) : begin_(begin), end_(begin + dim) {}
    tindexeddata(const_iterator begin, size_type dim) : begin_(begin.data()), end_(begin + dim) {}
    tindexeddata(const_iterator begin, const_iterator end) : begin_(begin.data()), end_(end.data()) {}
  private:
    iterator begin_;
    iterator end_;
  };
  
  template<
    typename T,
    typename PT = T*,
    bool weak = false,
    typename S = size_t,
    typename D = ptrdiff_t
  > struct tarray : tindexeddata<T, PT, S, D> {
    typedef typename tindexeddata<T, PT, S, D>::iterator iterator;
    typedef typename tindexeddata<T, PT, S, D>::size_type size_type;
    typedef typename tindexeddata<T, PT, S, D>::value_type value_type;
    typedef typename tindexeddata<T, PT, S, D>::reference reference;
    typedef typename tindexeddata<T, PT, S, D>::const_reference const_reference;

    tarray() : tindexeddata<T, PT, S> (nullptr, nullptr) {}
    
    tarray(const tarray& rhs) : tindexeddata<T, PT, S> (rhs.begin(), rhs.end()) {}
     
    tarray(iterator data, size_type n) 
      : tindexeddata<T, PT, S> (data, n) {}
    
    tarray(size_type n) : tindexeddata<T, PT, S> (iterator(new T[n]), n) {}
    
    ~tarray() {
      delete [] this->begin().data();
    }
  };
  
  template<
    typename T,
    typename PT,
    typename S,
    typename D
  > struct tarray<T, PT, true, S, D> : tindexeddata<T, PT, S, D> {
    typedef typename tindexeddata<T, PT, S, D>::iterator iterator;
    typedef typename tindexeddata<T, PT, S, D>::const_iterator const_iterator;
    typedef typename tindexeddata<T, PT, S, D>::size_type size_type;
    typedef typename tindexeddata<T, PT, S, D>::value_type value_type;
    typedef typename tindexeddata<T, PT, S, D>::reference reference;
    typedef typename tindexeddata<T, PT, S, D>::const_reference const_reference;

    tarray() : tindexeddata<T, PT, S, D> () {}

    tarray(const tarray& rhs) : tindexeddata<T, PT, S, D> (rhs.begin(), rhs.end()) {}
    
    tarray(const tarray<T, PT, false, S>& rhs) : tindexeddata<T, PT, S, D> (iterator(rhs.begin().data()), rhs.dim()) {}

    tarray(iterator data, size_type n) 
    : tindexeddata<T, PT, S> (data, n) {}

    tarray(const_iterator data, size_type n) 
    : tindexeddata<T, PT, S> (data, n) {}

  };
  
}
