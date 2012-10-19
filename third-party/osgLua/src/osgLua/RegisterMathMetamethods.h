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
#ifndef INCLUDED_RegisterMathMetamethods_h_GUID_2338d460_fd97_40ce_bff4_068da65d08bd
#define INCLUDED_RegisterMathMetamethods_h_GUID_2338d460_fd97_40ce_bff4_068da65d08bd

// Internal Includes
#include "OperatorMetamethodTraits.h"
#include "BinaryOperatorDispatch.h"
#include "boost/BinaryOperators.h"
#include "boost/MathAndArithmeticTypes.h"
#include "LuaIncludeFull.h"
#include "PrintInfoFunctor.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgLua {
	typedef osgTraits::BinaryOperators MathOperators;

	template<typename T, typename OperatorTag>
	struct PushOperator_impl {
		template<typename Operator>
		struct apply;
	};

	template<typename T, typename Operator>
	struct PushOperator : PushOperator_impl<T, typename osgTraits::get_operator_arity<Operator>::type>::template apply<Operator> {};

	template<typename T>
	struct PushOperator_impl<T, boost::mpl::int_<2> > {
		template<typename Operator>
		struct apply {
			static void doPush(lua_State * L) {
				boost::mpl::for_each<osgTraits::math_and_arithmetic_types>(PrintInfoFunctor<T, Operator>());
				lua_pushcfunction(L, &(attemptBinaryOperator<Operator, T>));
			}
		};
	};

	template<typename T>
	class RegisterOperatorFunctor {
		public:
			RegisterOperatorFunctor(lua_State * L) : _L(L) {}

			template<typename Operator>
			void operator()(Operator const&) const {
				PushOperator<T, Operator>::doPush(_L);
				lua_setfield(_L, -2, MetamethodName<Operator>::get());
				OSG_INFO << "Registered " << MetamethodName<Operator>::get() << std::endl;
			}
		private:
			lua_State * _L;
	};


	class RegisterOperators {
		public:
			RegisterOperators(lua_State * L, introspection::Type const& t) : _L(L), metatableType(t), found(false) {}
			template<typename T>
			void operator()(T const&) {
				//OSG_INFO << "In RegisterOperators with " << typeid(T).name() << std::endl;
				if (!found && introspection::Reflection::getType(extended_typeid<T>()) == metatableType) {
					OSG_INFO << "Pushing metafunctions for " << metatableType.getQualifiedName() << std::endl;
					boost::mpl::for_each<MathOperators>(RegisterOperatorFunctor<T>(_L));
					found = true;
				}
			}
		private:
			lua_State * _L;
			introspection::Type const& metatableType;
			bool found;
	};

	inline void registerMathMetamethods(lua_State * L, introspection::Type const& t) {
		boost::mpl::for_each<osgTraits::math_types>(RegisterOperators(L, t));
	}

} // end of namespace osgLua

#endif // INCLUDED_RegisterMathMetamethods_h_GUID_2338d460_fd97_40ce_bff4_068da65d08bd
