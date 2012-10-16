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
#include "boost/Multiplication.hpp"

// Library/third-party includes
#include <boost/mpl/quote.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/for_each.hpp>

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

	template<typename BoundOp>
	class BinaryOperatorApplicationFunctor {
		public:
			BinaryOperatorApplicationFunctor(lua_State * L, introspection::Type const& otherType) : other(otherType), a1(getValue(L, -2)), a2(getValue(L, -1)), r(), success(false) {}

			template<typename T>
			void operator()(boost::mpl::identity<T> const&) {
				if (!success && typeUsableAs<T>(other)) {
					typedef typename boost::mpl::apply<BoundOp, T>::type OpSpec;
					typedef typename OpSpec::first first;
					typedef typename OpSpec::second second;
					r = OpSpec::apply(introspection::variant_cast<first>(a1), introspection::variant_cast<second>(a2));
					success = true;
				}
			}

			int pushIfSuccessful(lua_State * L) const {
				if (success) {
					Value::push(L, r);
					return 1;
				}
				return 0;
			}

		private:
			introspection::Type const& other;
			introspection::Value a1;
			introspection::Value a2;
			introspection::Value r;
			bool success;
	};
	namespace {
		template<typename BoundOp>
		int attemptBoundBinaryOperator(lua_State * L, introspection::Type const& otherType) {
			typedef typename boost::mpl::apply<osgTraits::GetAvailableOtherArgTypes, BoundOp>::type OtherArgumentPossibilities;
			typedef BinaryOperatorApplicationFunctor<BoundOp> FunctorType;
			FunctorType f(L, otherType);
			boost::mpl::for_each<OtherArgumentPossibilities, boost::mpl::identity<boost::mpl::_1>,  FunctorType &>(f);
			return f.pushIfSuccessful(L);
		}

		template<typename Op, typename T1>
		int attemptBinaryOperator(lua_State * L) {
			if (osgLuaValueUsableAs<T1>(L, -2)) {
				typedef typename boost::mpl::apply<osgTraits::OperatorBindFirst, Op, T1>::type BoundOp;
				return attemptBoundBinaryOperator<BoundOp>(L, getValue(L, -1).getType());
			} else if (osgLuaValueUsableAs<T1>(L, -1) {
			typedef typename boost::mpl::apply<osgTraits::OperatorBindSecond, Op, T1>::type BoundOp;
			return attemptBoundBinaryOperator<BoundOp>(L, getValue(L, -2).getType());
			}
			return 0;
		}
	} // end of anonymous namespace


} // end of namespace osgLua

#endif // INCLUDED_BinaryOperatorDispatch_h_GUID_6c86a34e_7070_4d10_aa19_5ae304064960
