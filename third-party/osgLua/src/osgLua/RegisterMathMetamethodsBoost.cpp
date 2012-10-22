/**
	@file
	@brief Implementation

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

#ifdef OSGLUA_HAVE_BOOST

// Internal Includes
#include "RegisterMathMetamethods.h"
#include "boost/Operators.h"
#include "boost/MathAndArithmeticTypes.h"
#include "UnaryOperatorDispatch.h"
#include "BinaryOperatorDispatch.h"
#include "OperatorMetamethodTraits.h"
#include "PrintInfoFunctor.h"
#include "StatefulTypeVisitFunctor.h"
#include <osgLua/LuaInclude>
#include <osgLua/Value>

// Library/third-party includes
#include <boost/mpl/for_each.hpp>

// Standard includes
// - none

namespace osgLua {

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
		lua_pushcfunction(L, &(attemptUnaryOperator<Operator, T>));
		lua_setfield(L, -2, MetamethodName<Operator>::get());
	}

	template<typename T, typename Operator>
	inline void pushAndSetOperator(lua_State *, boost::mpl::false_ const&) {
		OSG_INFO << "Skipped " << MetamethodName<Operator>::get() << " since it's not applicable." << std::endl;
	}

	struct RegistrationData {
		RegistrationData(lua_State * _L, introspection::Type const& t) : L(_L), metatableType(t), foundOurType(false) {}
		lua_State * L;
		introspection::Type const& metatableType;
		bool foundOurType;
		Comparability comparability;
	};

	template<typename T>
	struct visit_types {
		template<typename Operator>
		struct visit_operator {
			//typedef typename boost::mpl::apply<boost::mpl::quote2<osgTraits::IsOperatorApplicable>, Operator, T>::type IsApplicable;
			static void visit(RegistrationData const& d) {
				OSG_INFO << "Registering " << MetamethodName<Operator>::get() << std::endl;
				pushAndSetOperator<T, Operator>(d.L, typename osgTraits::IsOperatorApplicable<Operator, T>::type());
			}
		};

		static void visit(RegistrationData & d) {
			//OSG_INFO << "In RegisterOperators with " << typeid(T).name() << std::endl;
			if (!d.foundOurType && introspection::Reflection::getType(extended_typeid<T>()) == d.metatableType) {
				OSG_INFO << "Pushing metafunctions for " << d.metatableType.getQualifiedName() << std::endl;
				d.foundOurType = true;
				d.comparability.eq = true;
				d.comparability.lt = true;
				d.comparability.le = typename osgTraits::is_vector<T>::type();

				boost::mpl::for_each<osgTraits::MathOperators, visit_operator<boost::mpl::_1> >(util::visitorState(d));
			}

		}
	};

	Comparability registerMathMetamethods(lua_State * L, introspection::Type const& t) {
		RegistrationData data(L, t);
		boost::mpl::for_each<osgTraits::math_types, visit_types<boost::mpl::_1> >(util::visitorState(data));
		return data.comparability;
	}

} // end of namespace osgLua

#endif
