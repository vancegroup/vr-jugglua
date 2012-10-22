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
#include "boost/Operators.h"
#include "boost/MathAndArithmeticTypes.h"
#include "BinaryOperatorDispatch.h"
#include "OperatorMetamethodTraits.h"
#include "PrintInfoFunctor.h"
#include "StatefulTypeVisitFunctor.h"
#include <osgLua/LuaInclude>

// Library/third-party includes
#include <boost/mpl/for_each.hpp>

// Standard includes
// - none

namespace osgLua {

	template<typename T, typename OperatorTag>
	struct PushAndSetOperator_impl {
		template<typename Operator>
		struct apply;
	};

	template<typename T, typename Operator>
	struct PushAndSetOperator : PushAndSetOperator_impl<T, typename osgTraits::get_operator_arity<Operator>::type>::template apply<Operator> {};

	template<typename T>
	struct PushAndSetOperator_impl<T, boost::mpl::int_<2> > {
		template<typename Operator>
		struct apply {
			static void doPush(lua_State * L) {
				boost::mpl::for_each<osgTraits::math_and_arithmetic_types>(PrintInfoFunctor<T, Operator>());
				lua_pushcfunction(L, &(attemptBinaryOperator<Operator, T>));
				lua_setfield(L, -2, MetamethodName<Operator>::get());
			}
		};
	};

	struct StateHolder {
		lua_State * L;
	};

	template<typename T, typename Operator>
	inline typename boost::enable_if<boost::mpl::equal_to<typename osgTraits::get_operator_arity<Operator>::type, boost::mpl::int_<2> > >::type
	pushAndSetOperator(lua_State * L, boost::mpl::true_ const&) {
		boost::mpl::for_each<osgTraits::math_and_arithmetic_types>(PrintInfoFunctor<T, Operator>());
		lua_pushcfunction(L, &(attemptBinaryOperator<Operator, T>));
		lua_setfield(L, -2, MetamethodName<Operator>::get());
	}
	template<typename T, typename Operator>
	inline typename boost::enable_if<boost::mpl::equal_to<typename osgTraits::get_operator_arity<Operator>::type, boost::mpl::int_<1> > >::type
	pushAndSetOperator(lua_State * L, boost::mpl::true_ const&) {
		OSG_INFO << "Unary operators not yet implemented: " << MetamethodName<Operator>::get() << std::endl;
	}
	/*
	template<typename T, typename Operator>
	inline void pushAndSetOperator(lua_State * L, boost::mpl::true_ const&) {
		PushAndSetOperator<T, Operator>::doPush(L);
		OSG_INFO << "Registered " << MetamethodName<Operator>::get() << std::endl;
	}
	*/

	template<typename T, typename Operator>
	inline void pushAndSetOperator(lua_State *, boost::mpl::false_ const&) {
		OSG_INFO << "Skipped " << MetamethodName<Operator>::get() << " since it's not applicable." << std::endl;
	}



	struct RegistrationData {
		RegistrationData(lua_State * L, introspection::Type const& t) : _L(L), metatableType(t), foundOurType(false) {}
		lua_State * _L;
		introspection::Type const& metatableType;
		bool foundOurType;
	};
	
	template<typename T>
	struct visit_types {
		template<typename Operator>
		struct visit_operator {
			//typedef typename boost::mpl::apply<boost::mpl::quote2<osgTraits::IsOperatorApplicable>, Operator, T>::type IsApplicable;
			static void visit(RegistrationData const& d) {
				OSG_INFO << "Registering " << MetamethodName<Operator>::get() << std::endl;
				pushAndSetOperator<T, Operator>(d._L, typename osgTraits::IsOperatorApplicable<Operator, T>::type());
			}
		};

		static void visit(RegistrationData & d) {
			//OSG_INFO << "In RegisterOperators with " << typeid(T).name() << std::endl;
			if (!d.foundOurType && introspection::Reflection::getType(extended_typeid<T>()) == d.metatableType) {
				OSG_INFO << "Pushing metafunctions for " << d.metatableType.getQualifiedName() << std::endl;
				d.foundOurType = true;
				boost::mpl::for_each<osgTraits::MathOperators, visit_operator<boost::mpl::_1> >(util::visitorState(d));
			}

		}
	};

/*
	class RegisterOperators {
		public:
			RegisterOperators(lua_State * L, introspection::Type const& t) : _L(L), metatableType(t), found(false) {}
			template<typename T>
			void operator()(T const&) {
				//OSG_INFO << "In RegisterOperators with " << typeid(T).name() << std::endl;
				if (!found && introspection::Reflection::getType(extended_typeid<T>()) == metatableType) {
					OSG_INFO << "Pushing metafunctions for " << metatableType.getQualifiedName() << std::endl;
					RegisterOperator<T>::doRegister(_L);
					found = true;
				}
			}
		private:
			lua_State * _L;
			introspection::Type const& metatableType;
			bool found;
	};
*/
	inline void registerMathMetamethods(lua_State * L, introspection::Type const& t) {
		RegistrationData data(L, t);
		boost::mpl::for_each<osgTraits::math_types, visit_types<boost::mpl::_1> >(util::visitorState(data));
	}

} // end of namespace osgLua

#endif // INCLUDED_RegisterMathMetamethods_h_GUID_2338d460_fd97_40ce_bff4_068da65d08bd
