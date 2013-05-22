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
#include "UsableAs.h"
#include "AttemptOperator.h"
#include "StatefulTypeVisitFunctor.h"

#include "LuaIncludeFull.h"

// Library/third-party includes
#include <osgTraits/InvokeOperator.h>
#include <osgTraits/IsOperatorAvailable.h>
#include <osgTraits/ConstructOperation.h>
#include <osgTraits/ArityTags.h>

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
		BinaryOpData(lua_State * _L, int otherStackIdx)
			: L(_L)
			, other(getValue(L, otherStackIdx).getType())
			, a1(getValue(L, -2))
			, a2(getValue(L, -1))
			, r()
			, otherIdx(otherStackIdx)
			, isOtherNumber(static_cast<bool>(lua_isnumber(L, otherStackIdx)))
			, success(false) {}
		lua_State * L;
		introspection::Type const& other;
		introspection::Value a1;
		introspection::Value a2;
		introspection::Value r;
		int otherIdx;
		bool isOtherNumber;
		bool success;
		int pushIfSuccessful(lua_State * L) const {
			if (success) {
				Value::push(L, r);
				return 1;
			}
			return 0;
		}
	};

	namespace {
		template<typename Operation>
		inline void performBinaryOperation(BinaryOpData & d) {
			typedef typename osgTraits::get_operation_argument_c<Operation, 0>::type T1;
			typedef typename osgTraits::get_operation_argument_c<Operation, 1>::type T2;
			d.r = osgTraits::invokeOperation<Operation>(introspection::variant_cast<T1>(d.a1), introspection::variant_cast<T2>(d.a2));
			d.success = true;
		}
	}// end of namespace

	namespace ArgumentLocations {
		enum {
			FIRST = 0,
			SECOND = 1
		};
	}


	template<typename Operator, typename T>
	struct AttemptOperator<Operator, T, osgTraits::arity_tags::binary_tag> {
		template<int BindArg>
		struct AttemptOrder {
			enum {
				BoundArg = (BindArg  == ArgumentLocations::FIRST ? -2 : -1),
				OtherArg = (BindArg  == ArgumentLocations::SECOND ? -2 : -1)
			};
			typedef typename osgTraits::construct_bound_operation<Operator, T, BindArg>::type BoundOp;
			typedef typename osgTraits::get_valid_other_arg_types<BoundOp>::type OtherArgumentPossibilities;

			template<typename T2>
			struct other_argument_visitor {
				static void visit(BinaryOpData & d) {
					if (!d.success && osgLuaValueUsableAs<T2>(d.L, d.otherIdx)) {
						typedef typename osgTraits::add_argtype<BoundOp, T2>::type Operation;
						performBinaryOperation<Operation>(d);
					}
				}
			};
			static bool checkAndRun(lua_State * L, int & ret) {
				if (osgLuaValueUsableAs<T>(L, BoundArg)) {
					BinaryOpData data(L, OtherArg);
					boost::mpl::for_each<OtherArgumentPossibilities, other_argument_visitor<boost::mpl::_1> >(util::visitorState(data));
					ret = data.pushIfSuccessful(L);
					return true;
				}
				return false;
			}

		};

		static int attempt(lua_State * L) {
			int ret = 0;
			if (lua_isnil(L, -2) || lua_isnil(L, -1)) {
				return luaL_error(L, "[%s:%d] Could not %s: %s operand is nil", __FILE__, __LINE__,
				                  osgTraits::OperatorVerb<Operator>::get(), (lua_isnil(L, -2) ? "first" : "second"));
			}
			AttemptOrder<ArgumentLocations::FIRST>::checkAndRun(L, ret) || AttemptOrder<ArgumentLocations::SECOND>::checkAndRun(L, ret);
			if (ret == 0) {
				return luaL_error(L, "[%s:%d] Could not %s instances of %s, %s", __FILE__, __LINE__,
				                  osgTraits::OperatorVerb<Operator>::get(), getValue(L, -2).getType().getQualifiedName().c_str(), getValue(L, -1).getType().getQualifiedName().c_str());
			}
			return ret;
		}
	};


} // end of namespace osgLua

#endif // INCLUDED_BinaryOperatorDispatch_h_GUID_6c86a34e_7070_4d10_aa19_5ae304064960
