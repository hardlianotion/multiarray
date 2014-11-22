/*
 *      staticcontrolstructures.h
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

 //These are  control structures that are resolved in compile time, rather than
//runtime.  

//Compile If
namespace BASIC {

	template<
		int condition,
		class Then,
		class Else
	> struct IF {

		typedef Then RESULT;
	};

	template<
		class Then,
		class Else
	> struct IF<
		0,
		Then,
		Else
	> {
		typedef Else RESULT;
	};

	//This is a lazy version of the above

	template<
		int condition,
		typename Then,
		typename Else
	> struct LazyIF : IF< condition, Then, Else >::RESULT {
	
	};

	//WHILE loop helpers

	template <
		class Statement
	> struct STOP {
		typedef Statement RESULT;
	};

	//WHILE loop

	template <
		class Condition,
		class Statement
	> struct WHILE {
		typedef typename
				IF<
					Condition::template Code<Statement>::RESULT,
					WHILE<Condition,typename Statement::Next>,
					STOP<Statement>
				>::RESULT::RESULT RESULT;
	};

	//DO loop
	template <
		class Condition,
		class Statement
	> class DO {

		typedef typename Statement::Next NewStatement;
	
	public:

		typedef typename
			IF<
				Condition::template Code<NewStatement>::RESULT,
				DO< Condition, NewStatement>,
				STOP<NewStatement>
			>::RESULT::RESULT RESULT;
	};

	// comparators for FOR loop.

	struct Greater {
		template<
			int x,
			int y
		> struct Code {
			enum {RESULT = (x > y) };
		};
	};

	struct GreaterEqual {
		template<
			int x,
			int y
		> struct Code {
			enum {RESULT = (x >= y) };
		};
	};

	struct Less {
		template<
			int x,
			int y
		> struct Code {
			enum {RESULT = (x < y) };
		};
	};

	struct LessEqual {
		template<
			int x,
			int y
		> struct Code {
			enum {RESULT = (x <= y) };
		};
	};

  template <
		int from,
		class Compare,
		int to,
        int by,
		class Statement
	 > struct FOR {

		typedef typename
		IF<
			Compare::template Code<from,to>::RESULT,
			FOR<
				from+by,
				Compare,
				to,
				by,
				typename Statement::template Code<from>::Next
			>,
			STOP<typename Statement::template Code<from> >
			>::RESULT::RESULT RESULT;

	};
	
	const int DEFAULT = ~(~0u >> 1);	//smallest integer.

	struct NilCase {
	};

	template <
		int arg_tag,
		class ArgType,
		class ArgNext = NilCase
	> struct CASE {
		
		enum{tag = arg_tag};

		typedef ArgType Type;
		typedef ArgNext Next;
	};

	template<
		int tag,
		class Case
	> class SWITCH {

	typedef typename Case::Next NextCase;

	enum{
		case_tag = Case::tag,
		found = (case_tag == tag || case_tag == DEFAULT )
	};

	public:

		typedef typename IF<
			found,
			typename Case::Type,
			typename SWITCH<tag, NextCase>::RESULT
		>::RESULT RESULT;
	
	};

	template<
		int tag
	> class SWITCH<
		tag,
		NilCase
	> {

	public:
		typedef NilCase RESULT;
	};	

} //marray

