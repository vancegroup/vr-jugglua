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
#ifndef INCLUDED_BinaryOperatorDispatch_h_GUID_6c86a34e_7070_4d10_aa19_5ae304064960
#define INCLUDED_BinaryOperatorDispatch_h_GUID_6c86a34e_7070_4d10_aa19_5ae304064960

// Internal Includes
#include "boost/BinaryOperators.h"
#include "boost/InvokeOperator.h"

#include "UsableAs.h"
#include "StatefulTypeVisitFunctor.h"

#include "LuaIncludeFull.h"

// Library/third-party includes
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/type_traits/add_reference.hpp>

#include <osgLua/introspection/ExtendedTypeInfo>
#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>
#include <osgLua/introspection/variant_cast>

#include <osg/Notify>

// Standard includes
// - none

namespace osgLua {

	struct BinaryOpData {
		BinaryOpData(lua_State * L, int otherStackIdx)
			: other(getValue(L, otherStackIdx).getType())
			, a1(getValue(L, -2))
			, a2(getValue(L, -1))
			, r()
			, success(false) {}
		introspection::Type const& other;
		introspection::Value a1;
		introspection::Value a2;
		introspection::Value r;
		bool success;
		int pushIfSuccessful(lua_State * L) const {
			if (success) {
				Value::push(L, r);
				return 1;
			}
			return 0;
		}
	};

	template<typename BoundOp>
	struct AttemptBoundBinaryOperator {
		template<typename T>
		struct visit_binary_op_application {
			static void visit(BinaryOpData & d) {
				if (!d.success && typeUsableAs<T>(d.other)) {
					typedef typename boost::mpl::apply<BoundOp, T>::type OpSpec;
					typedef typename osgTraits::get_operator_argument_c<OpSpec, 0>::type first;
					typedef typename osgTraits::get_operator_argument_c<OpSpec, 1>::type second;
					d.r = osgTraits::invokeOperator<OpSpec>(introspection::variant_cast<first>(d.a1), introspection::variant_cast<second>(d.a2));
					d.success = true;
				}
			}
		};

		typedef typename osgTraits::get_valid_other_arg_types<BoundOp>::type OtherArgumentPossibilities;
		static int attempt(lua_State * L, int otherStackIdx) {
			BinaryOpData data(L, otherStackIdx);
			boost::mpl::for_each<OtherArgumentPossibilities, visit_binary_op_application<boost::mpl::_1> >(util::visitorState(data));
			return data.pushIfSuccessful(L);
		}
	};

	namespace {
		template<typename Op, typename T1>
		int attemptBinaryOperator(lua_State * L) {
			int ret = 0;
			if (lua_isnil(L, -2) || lua_isnil(L, -1)) {
				return luaL_error(L, "[%s:%d] Could not %s: %s operand is nil", __FILE__, __LINE__,
				                  osgTraits::OperatorVerb<Op>::get(), (lua_isnil(L, -2) ? "first" : "second"));
			}
			if (osgLuaValueUsableAs<T1>(L, -2)) {
				typedef typename osgTraits::operator_bind_first<Op, T1>::type BoundOp;
				typedef AttemptBoundBinaryOperator<BoundOp> AttemptStruct;

				ret = AttemptStruct::attempt(L, -1);
			} else if (osgLuaValueUsableAs<T1>(L, -1)) {
				typedef typename osgTraits::operator_bind_second<Op, T1>::type BoundOp;
				typedef AttemptBoundBinaryOperator<BoundOp> AttemptStruct;

				ret = AttemptStruct::attempt(L, -2);
			}
			if (ret == 0) {
				return luaL_error(L, "[%s:%d] Could not %s instances of %s, %s", __FILE__, __LINE__,
				                  osgTraits::OperatorVerb<Op>::get(), getValue(L, -2).getType().getQualifiedName().c_str(), getValue(L, -1).getType().getQualifiedName().c_str());
			}
			return ret;
		}
	} // end of anonymous namespace


} // end of namespace osgLua

#endif // INCLUDED_BinaryOperatorDispatch_h_GUID_6c86a34e_7070_4d10_aa19_5ae304064960
