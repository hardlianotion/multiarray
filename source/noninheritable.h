/*
 *      noninheritable.h
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

namespace BASIC {
/**
The following works (tnon_inheritable compiles to a class that does not permit inheritance operations from it),
but the resulting error message when no compilation occurs is horrible.  Note that no implementation of
BasicallyInheritable can exist by itself, due to the private default constructor.  The public interface for tnon_inheritable
is basically that of the parameter class NoInheritClass.

Use tnon_inheritable if you want to construct a class that cannot be subclassed, using an already existing class
(without altering the class itself).

Use tbase_no_inherit_class if you have a class that you want to prevent others from subclassing.  Use this by inheriting
private virtually from tbase_no_inherit_class.  The template parameter to this class is purely for error message purposes so that
one can find the class being inheritance-controlled.
*/

template <
    class C
> struct tnon_inheritable;

template <
    class C
> class tbasically_uninheritable : public virtual C{

    friend struct tnon_inheritable<C>;

    tbasically_uninheritable(
    ) {
    
    }
  
};

template <
    class C 
> struct tnon_inheritable : public virtual tbasically_uninheritable<C>{

   tnon_inheritable(
   ) : tbasically_uninheritable<C>() {
   }

};

/**
Inheriting virtually from this class prevents it from being subclassed.

Requires a C default constructor.
*/

template <
    class C
> class tbase_no_inherit_class {

protected:

    //prevents construction except through inheritance
   tbase_no_inherit_class(
   ) {
    C();
   }
   
};

}; 

 
