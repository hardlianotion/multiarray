/*
 *      interpolator.h
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
#include <map>

namespace BASIC {
	
	template <
		typename DT,
		typename IM
	> struct tinterpolator : IM::spec {
		
		typedef DT type;
		typedef IM method;
		
		typedef std::map< typename method::domain, typename method::range > map;
		typedef std::map< typename method::range, typename method::domain > inv_map;
		
		
	};
}