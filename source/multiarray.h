/*
 *      multiarray.h
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
#include "arrayiterator.h"
#include "arraylayouts.h"
#include "array.h"
#include <vector>

namespace marray {
    using std::array;
    
    template<
        typename T, 
        size_t N,
        typename PT = T*,
        typename S = size_t,
        typename D = ptrdiff_t,
        bool W = false,
        typename L = trectlayout<N, S, D>
   > struct tmultiarray: tindexeddata<T, PT, S, D> {
        
        typedef array<S, N> index_type;
        typedef L layout_type;
        typedef tindexeddata<T, PT, S, D> base_array;
        typedef typename base_array::size_type size_type;
        typedef typename base_array::reference reference;
        typedef typename base_array::const_reference const_reference;
        friend  struct tmultiarray<T, N - 1, PT, S, D, true, typename L::slice_layout>;
        typedef tmultiarray<T, N - 1, PT, S, D, true, typename L::slice_layout> slice_type;
        typedef typename base_array::iterator iterator;
        
        enum{ RANK = N };
        
        tmultiarray() {}
        
        tmultiarray(const tmultiarray& rhs) 
            : base_array(rhs), layout_(rhs.layout_), slice_ref_() {}
        
        tmultiarray(
            typename base_array::iterator begin, 
            const layout_type& layout 
       ) : base_array(begin, layout.footprint()), layout_(layout), slice_ref_(){}
        
        const_reference
        operator()(const index_type& idx) const {
            return base_array::operator[](layout_.get_stride(idx));
        }
        
        reference
        operator()(const index_type& idx) {
            return base_array::operator[](layout_.get_stride(idx));
        }
        
        const slice_type&
        operator[](size_type i) const {
            assert(i <dim(i));
            this->slice_ref_.reset(this->begin() + i * layout_.slice(0).footprint(), layout_.slice(0));
            return slice_ref_;
        }
        
        slice_type&
        operator[](size_type i) {
            assert(i <dim(i));

            this->slice_ref_.reset(this->begin() + i * layout_.slice(0).footprint(), layout_.slice(0));
            return slice_ref_;
        }
        
        /**
        dim
        
        returns the dimension along the i axis.
        */
        size_t
        dim(size_t i) const { return layout_.dim(i); }
            
        /**
        layout

        permits interrogation of the array's dimensional structure.
        */
        const layout_type&
        layout() const {
            return layout_;
        }
        /**
        reset
        
        sets up a new beginning and layout for the multiarray.
        */
        void
        reset(iterator begin, const layout_type& layout) {
            layout_ = layout;
            reset(begin);
        }
        
        /**
        reset
        
        sets up a new beginning for the multiarray.
        */
        void reset(iterator begin) {
            this->reset(begin, begin + layout_.footprint());
        }
        
        protected:
        
        layout_type layout_;
        mutable slice_type slice_ref_;
    };
    
    template<
        typename T, 
        typename PT,
        typename S,
        typename D,
        bool W,
        typename L
   > struct tmultiarray<T, 2, PT, S, D, W, L> : tindexeddata<T, PT, S, D> {
        
        typedef array<S, 2> index_type;
        typedef L layout_type;
        typedef tindexeddata<T, PT, S, D> base_array;
        typedef typename base_array::size_type size_type;
        typedef typename base_array::reference reference;
        typedef typename base_array::const_reference const_reference;
        typedef tarray<T, PT, true, S, D> slice_type;
        typedef typename base_array::iterator iterator;
        
        enum{ RANK = 2 };
        
        tmultiarray() {}
        
        tmultiarray(const tmultiarray& rhs) 
            : base_array(rhs), layout_(rhs.layout_), slice_ref_() {}
        
        tmultiarray(
            typename base_array::iterator begin, 
            const layout_type& layout 
       ) : base_array(begin, layout.footprint()), layout_(layout), slice_ref_() {}
        
        const_reference
        operator()(const index_type& idx) const {
            return this->operator[](layout_.get_stride(idx));
        }
        
        reference
        operator()(const index_type& idx) {
            return this->operator[](layout_.get_stride(idx));
        }
        
        const slice_type&
        operator[](size_type i) const {
            assert(i <dim(i));
            slice_ref_.reset(this->begin() + i * layout_.slice(0).footprint(), this->begin() + i * layout_.slice(0).footprint() + layout_.dim(1));
            return slice_ref_;
        }
        
        slice_type&
        operator[](size_type i) {
            assert(i <dim(i));
            this->slice_ref_.reset(this->begin() + i * layout_.slice(0).footprint(), this->begin() + i * layout_.slice(0).footprint() + layout_.dim(1));
            return slice_ref_;
        }
        
        /**
        dim
        
        returns the dimension along the i axis.
        */
        size_t
        dim(size_t i) const { return layout_.dim(i); }
            
        /**
        layout

        permits interrogation of the array's dimensional structure.
        */
        const layout_type&
        layout() const {
            return layout_;
        }

        /**
        reset
        
        sets up a new beginning and layout for the multiarray.
        */
        void
        reset(iterator begin, const layout_type& layout) {
            layout_ = layout;
            reset(begin);
        }
        
        /**
        reset
        
        sets up a new beginning for the multiarray.
        */
        void reset(iterator begin) {
            tindexeddata<T, PT, S, D>::reset(begin, begin + layout_.footprint());
        }
    protected:
        
        layout_type layout_;
        mutable slice_type slice_ref_;
    };
        
    template<
        typename T, 
        size_t N,
        typename PT,
        typename S,
        typename D,
        typename L
   > struct tmultiarray<T, N, PT, S, D, false, L>  : tmultiarray<T, N, PT, S, D, true, L> {
        typedef tmultiarray<T, N, PT, S, D, true, L> base_array;
        typedef typename base_array::index_type index_type;
        typedef typename tmultiarray<T, N, PT, S, D, true, L>::size_type size_type;
        typedef typename tmultiarray<T, N, PT, S, D, true, L>::reference reference;
        typedef tmultiarray<T, N - 1, PT, S, D, true, L> slice_type;
        typedef typename base_array::iterator iterator;
        typedef L layout_type;
        
        tmultiarray(const tmultiarray& rhs) {}
        
        tmultiarray(const tmultiarray<T, N, PT, S, D, true, L>& rhs) {}
        
        tmultiarray(const typename base_array::layout_type& layout) 
            : base_array(new T[layout.footprint()], layout) {}        
            
        /**
        dim
        
        returns the dimension along the i axis.
        */
        size_t
        dim(size_t i) const { return this->layout_.dim(i); }
        
        /**
        reset
        
        sets up a new beginning and layout for the multiarray.
        */
        void
        reset(iterator begin, const layout_type& layout) {
            //_layout = layout;
            //reset(begin);
        }
        
        /**
        reset
        
        sets up a new beginning for the multiarray.
        */
        void reset(iterator begin) {
            //this->reset(begin, begin + layout_.footprint());
        }
    };
    
    template<
        typename T, 
        typename PT,
        typename S,
        typename D,
        typename L
   > struct tmultiarray<T, 2, PT, S, D, false, L> : tmultiarray<T, 2, PT, S, D, true, L> {
        typedef tmultiarray<T, 2, PT, S, D, true, L> base_array;
        typedef typename base_array::index_type index_type;
        typedef typename tmultiarray<T, 2, PT, S, D, true, L>::size_type size_type;
        typedef typename tmultiarray<T, 2, PT, S, D, true, L>::reference reference;
        typedef tarray<T, PT, true, S, D> slice_type;
        typedef typename base_array::iterator iterator;
        typedef L layout_type;

        
        tmultiarray(const tmultiarray& rhs) {}
        
        tmultiarray(const tmultiarray<T, 2, PT, S, D, true, L>& rhs) {}
        
        tmultiarray(const typename base_array::layout_type& layout) 
            : base_array(new T[layout.footprint()], layout) {}        
            
        /**
        dim
        
        returns the dimension along the i axis.
        */
        size_t
        dim(size_t i) const { return this->layout_.dim(i); }
            
        /**
        reset
        
        sets up a new beginning and layout for the multiarray.
        */
        void
        reset(iterator begin, const layout_type& layout) {
            //_layout = layout;
            //reset(begin);
        }
        
        /**
        reset
        
        sets up a new beginning for the multiarray.
        */
        void reset(iterator begin) {
            //this->reset(begin, begin + layout_.footprint());
        }
    };

    
}
