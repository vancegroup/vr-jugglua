/** @file
	@brief Header

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_UnaryOperatorDispatch_h_GUID_4e4b8cb2_5d4d_43fe_9e91_291fa5c8d24f
#define INCLUDED_UnaryOperatorDispatch_h_GUID_4e4b8cb2_5d4d_43fe_9e91_291fa5c8d24f

// Internal Includes
#include "AttemptOperator.h"
#include <osgLua/Value>
#include <osgLua/introspection/Value>
#include <osgTraits/InvokeOperator.h>
#include <osgTraits/ConstructOperation.h>

#include "UsableAs.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgLua {

	template<typename Operator, typename T>
	struct AttemptOperator<Operator, T, osgTraits::arity_tags::unary_tag> {
		typedef typename osgTraits::construct_operation<Operator, T>::type Operation;
		static int attempt(lua_State * L) {
			if (osgLuaValueUsableAs<T>(L, -1)) {
				introspection::Value ret = osgTraits::invokeOperation<Operation>(introspection::variant_cast<T>(getValue(L, -1)));
				Value::push(L, ret);
				return 1;
			}
			return 0;
		}
	};

} // end of namespace osgLua
#endif // INCLUDED_UnaryOperatorDispatch_h_GUID_4e4b8cb2_5d4d_43fe_9e91_291fa5c8d24f
