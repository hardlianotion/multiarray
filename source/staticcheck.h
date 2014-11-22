/*
 *      staticcheck.h
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

#include "noninheritable.h"
#include <cstddef>



namespace BASIC {

    template<
        size_t condition,
        typename T
    > struct tstatic_assertion {
        enum { assertion_failure = sizeof( int[ condition ] ) };
    };

    // Some code
     
#define STATIC_ASSERT( condition, Message ) \
    struct ERROR_##Message{}; \
    enum{ static_##Message = BASIC::tstatic_assertion< condition, ERROR_##Message >::assertion_failure }

    /**
    This allows us to compare the type of two types at compile time.  Based on Alexandrescu's check for downcast
    safety  (Sutter : More Exceptional C++, item  4).  All that is needed is to ensure that no inheritance is permitted from either type, and
    we have what we want...

    Returns true if TA and TB are the same, false otherwise.
    */
     
    template <
      typename TA,
      typename TB
    > class tare_the_same {

        template <typename T> struct dummy{};
        /**
        This allows us to make sure that no inheritance is involved, 
        and allows us to use non-class/struct types
        */
        typedef dummy<TA> non_A;
        typedef dummy<TB> non_B;

        //Note: not implemented.
        struct no{};
        struct yes{ no m_no[2]; };

        static yes check( non_A* );
        static no check(...);

        public:

        enum{ TEST =  ( sizeof( check(static_cast< non_B* >(0) ) ) == sizeof(yes) ) };
      
    };
    /**
    Dunno for sure (lookup, mibbe), but this is probably the original 
    downcast version of the downcast check.

    This checks if TB is inherited from TA.  Returns true if it is, false otherwise.
    */
    template <
      typename TA,
      typename TB
    > class tis_inherited_from {

        //Note: not implemented.
        struct no{};
        struct yes{ no m_no[2]; };

        static yes check( TA* );
        static no check(...);

        public:
        //dummy function declared to stop processor warning.
        bool dummy();

        enum{ TEST =  ( sizeof( check(static_cast< TB* >(0) ) ) == sizeof(yes) ) };
    };

/**
    Enforce constraint that D must be the same as, or inherit from, B.

    From "Imperfect C++", 2005, Matthew Wilson.
*/
    template<
        typename D,
        typename B
    > struct tmust_have_base {

        ~tmust_have_base() {
            void(*p)( ) = constraint;
        }

    private:
            static void 
        constraint( ) {
            D* derived; 
            B* pB = base;
        }
    };

/**
 tis_polymorphic::TEST is false if T does not have a virtual base, or possess a virtual destructor
 and true otherwise.
*/

    template<
        typename T
    > class tis_polymorphic {

        struct dummy1 : public T {
            virtual ~dummy1(){} 
        };

        struct dummy2 : public T {
            ~dummy2(){}
        };

    public:
        enum{ TEST = sizeof(dummy1) == sizeof(dummy2) };
    };
  
} //marray


