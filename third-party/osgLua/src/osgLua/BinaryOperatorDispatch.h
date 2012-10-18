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

// Library/third-party includes
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/filter_view.hpp>

#include <osgLua/introspection/ExtendedTypeInfo>
#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>
#include <osgLua/introspection/variant_cast>

#include <osg/Notify>

// Standard includes
// - none

namespace osgLua {

	template<typename T>
	inline bool typeUsableAs(const introspection::Type& t) {
		static const introspection::Type& myType =
		    introspection::Reflection::getType(extended_typeid<T>());
		return t == myType || t.isSubclassOf(myType);
	}

	template<typename T>
	inline bool osgLuaValueUsableAs(lua_State * L, int i) {
		Value * a = Value::get(L, i);
		if (a) {
			return typeUsableAs<T>(a->get().getType());
		}
		return false;
	}

	struct BinaryOpData {
		BinaryOpData(lua_State * L, introspection::Type const& otherType) : other(otherType), a1(getValue(L, -2)), a2(getValue(L, -1)), r(), success(false) {}
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
	class BinaryOperatorApplicationFunctor {
		public:
			BinaryOperatorApplicationFunctor(BinaryOpData & data) : d(data) {}

			template<typename T>
			void operator()(T const&) {
				if (!d.success && typeUsableAs<T>(d.other)) {
					typedef typename boost::mpl::apply_wrap1<BoundOp, T>::type OpSpec;
					typedef typename osgTraits::first_argument_type<OpSpec>::type first;
					typedef typename osgTraits::second_argument_type<OpSpec>::type second;
					d.r = OpSpec::performOperation(introspection::variant_cast<first>(d.a1), introspection::variant_cast<second>(d.a2));
					d.success = true;
				}
			}



		private:
			BinaryOpData & d;
	};
	namespace {
		template<typename BoundOp>
		int attemptBoundBinaryOperator(lua_State * L, introspection::Type const& otherType) {
			//typedef typename osgTraits::GetAvailableOtherArgTypes<BoundOp>::type OtherArgumentPossibilities;
			//typedef osgTraits::math_types OtherArgumentPossibilities;
			BinaryOpData data(L, otherType);
			std::cout << "BoundOp " << typeid(BoundOp).name() << std::endl;
			std::cout << "is_bound_operator_available for osg::Vec3d" << typeid(typename osgTraits::is_bound_operator_available<BoundOp, osg::Vec3d>::type).name()  << std::endl;
			//boost::mpl::for_each<OtherArgumentPossibilities>(BinaryOperatorApplicationFunctor<BoundOp>(data));
			return data.pushIfSuccessful(L);
		}

		template<typename Op, typename T1>
		int attemptBinaryOperator(lua_State * L) {
			if (osgLuaValueUsableAs<T1>(L, -2)) {
				typedef typename osgTraits::OperatorBindFirst<Op, T1>::type BoundOp;
				return attemptBoundBinaryOperator<BoundOp>(L, getValue(L, -1).getType());
			} else if (osgLuaValueUsableAs<T1>(L, -1)) {
				typedef typename osgTraits::OperatorBindSecond<Op, T1>::type BoundOp;
				return attemptBoundBinaryOperator<BoundOp>(L, getValue(L, -2).getType());
			}
			return 0;
		}
	} // end of anonymous namespace


} // end of namespace osgLua

#endif // INCLUDED_BinaryOperatorDispatch_h_GUID_6c86a34e_7070_4d10_aa19_5ae304064960
