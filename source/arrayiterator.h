/*
 *      arrayiterator.h
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
#include <cassert>

namespace marray {
    using std::array;

    /**
    data - EXPERIMENTAL
    inputs - ptr
    
    Function to expose the underlying pointer data of an iterator.  Works
    transparently on raw pointers and iterators, expecting a data() method 
    on the latter.
    */
    template<
        typename T
    > T* data(T* ptr) { return ptr; }

    template<
        typename T
    > const T* data(const T* ptr) { return ptr; }
    
    template<
        typename T
    > const typename T::pointer_type data(const T& ptr) { return ptr.data(); }
    
    /**
    stride - EXPERIMENTAL
    inputs - ptr
    
    The number of quantum iterations per jump of this particular iterator.
    */
    
    template<
        typename T
    > size_t stride(T* ptr) { return 1; }
    
    template<
        typename T
    > size_t stride(const T* ptr) { return 1; }
    
    template<
        typename T
    > typename T::size_type stride(const T& ptr) { return ptr.stride(); }
    
    /**
    strides - EXPERIMENTAL
    inputs - begin, end
    
    Function to expose the number of iterations between begin and end parameters.
    */
    template<
        typename T
    > ptrdiff_t strides(T* begin, T* end) { return end - begin; }
    
    template<
        typename T
    > ptrdiff_t strides(const T* begin, const T* end) { return end - begin; }
    
    template<
        typename T
    > typename T::difference_type strides(const T& begin, const T& end) { 
        assert(stride(end) == stride(begin)); 
        return (end - begin) / stride(begin); 
    }
    
    template<
        typename T,
        typename PT,
        typename S = size_t,
        typename D = ptrdiff_t
    > struct titerator {
        typedef T value_type;
        typedef PT pointer_type;
        typedef S size_type;
        typedef D difference_type;
        
        titerator(PT ptr) : ptr_(ptr) {}
        titerator(const titerator& rhs): ptr_(rhs.ptr_) {}
 
        operator PT() { return ptr_; }
        
        titerator& 
        operator++() { ++ptr_; return *this; }
        
        titerator& 
        operator--() { --ptr_; return *this; }
        
        titerator 
        operator--(int) { titerator result(*this); --ptr_; return result; }
        
        titerator 
        operator++(int) { titerator result(*this); ++ptr_; return result; }
        
        titerator&
        operator+=(difference_type n) { ptr_ += n; return *this; }

        titerator&
        operator-=(difference_type n) { ptr_ -= n; return *this; }
        
        titerator
        operator+(difference_type n) const { titerator result(*this); return result += n; }

        titerator
        operator-(difference_type n) const { titerator result(*this); return result -= n; }
        
        difference_type
        operator-(const titerator& rhs) const { return strides(rhs.ptr_, ptr_);  }
        
        difference_type
        stride() const { return marray::stride(ptr_); } //weird - marray::stride to prevent compiler complaining about titerator::stride.
        
        T&
        operator*() { return *ptr_; }
        
        PT
        data() { return ptr_; }
        
        PT
        data() const { return ptr_; }
    private:

        PT ptr_;        
    };
    
    /**
    tconst - EXPERIMENTAL
    
    This is a wrapper class to provide a const_iterator, given an 
    iterator object.  
    */
    template<
        typename IT
    > struct tconst : private virtual IT {
        
        typedef typename IT::difference_type difference_type;
        typedef const typename IT::pointer_type pointer_type; 
        typedef typename IT::size_type size_type;
        typedef const typename IT::value_type value_type;

        tconst(pointer_type ptr) : IT(ptr) {}
        tconst(IT ptr): IT(ptr) {}
        
        tconst& 
        operator++() { IT::operator++(); return *this;}
        
        tconst& 
        operator--() { IT::operator--(); return *this;}
        
        tconst 
        operator--(int) { return tconst(IT::operator--(1)); }
        
        tconst 
        operator++(int) { return tconst(IT::operator++(1)); }
        
        tconst&
        operator+=(difference_type n) { IT::operator+=(n); return *this; }

        tconst&
        operator-=(difference_type n) { IT::operator-=(n); return *this; }
        
        tconst
        operator+(difference_type n) const { IT::operator+(n); return *this;}

        tconst
        operator-(difference_type n) const { IT::operator-(n); return *this;}
        
        difference_type
        operator-(const tconst& rhs) const { return IT::operator-(rhs); }
        
        difference_type
        stride() const { return IT::stride(); } //weird - marray::stride to prevent compiler complaining about tconst::stride.
        
        value_type
        operator*() { return IT::operator*(); }
        
        pointer_type
        data() const { return IT::data(); }
    };
    /**
    tstrideiterator - EXPERIMENTAL
    
    This is an iterator in contiguous memory that skips a (given) stride
    when iterating through an array container.
    */
    template<
        typename IT1,
        typename IT2
    > bool operator==(IT1 ptr1, IT2 ptr2) { return data(ptr1) == data(ptr2); }
    
    template<
        typename IT1,
        typename IT2
    > bool operator!=(IT1 ptr1, IT2 ptr2) { return data(ptr1) == data(ptr2); }
    
    template<
        typename T,
        typename PT,
        typename S,
        typename D
    > struct tstrideiterator : titerator<T, PT, S, D> {
        typedef typename titerator<T, PT, S, D>::difference_type difference_type;
        typedef typename titerator<T, PT, S, D>::pointer pointer;
        typedef typename titerator<T, PT, S, D>::size_type size_type;
        
        tstrideiterator(const tstrideiterator& rhs) : _base(rhs._base), _stride(rhs._stride){}
        
        tstrideiterator(const pointer base) : _base(base), _stride(1){}
        
        tstrideiterator(const pointer base, size_type stride) : _base(base), _stride(stride){}
        
        tstrideiterator() : _base(NULL), _stride(0) {}
        
        tstrideiterator&
        operator++() { return _base += _stride; }
        
        tstrideiterator&
        operator--() { return _base -= _stride; }
        
        tstrideiterator
        operator ++(int) { 
            tstrideiterator result(*this); ++(*this); return result; 
        }
        
        tstrideiterator
        operator --(int) { 
            tstrideiterator result(*this); --(*this); return result; 
        }
        
        tstrideiterator&
        operator+=(difference_type i) { return _base += _stride * i; }
        
        tstrideiterator&
        operator-=(difference_type i) { return _base -= _stride * i; }
        
        tstrideiterator
        operator + (difference_type i) const { 
            tstrideiterator result(*this); return result += i; 
        }
        
        tstrideiterator
        operator - (difference_type i) const { 
            tstrideiterator result(*this); return result -= i; 
        }
        
        size_type
        stride() const { return _stride; }
        
        pointer
        data() const { return data(_base); }
        
    private:
        pointer _base;
        size_type _stride;        
    };
    
    /**
    tsubspaceiterator - EXPERIMENTAL
    
    Allows correct iteration on slices taken along arbitrary axes in a multiarray.
    */
    template<
        typename T,
        typename PT,
        typename S, 
        typename D,
        size_t N
    > struct tsubspaceiterator : titerator<T, PT, S, D> {
        typedef array<S, N> slice_index;
        
        typedef typename titerator<T, PT, S, D>::difference_type difference_type;
        typedef typename titerator<T, PT, S, D>::pointer pointer;
        typedef typename titerator<T, PT, S, D>::size_type size_type;
        
        tsubspaceiterator() { }
        tsubspaceiterator(const tsubspaceiterator& rhs) { }
        //tsubspaceiterator() { }
    private:
        
    };
    
}
