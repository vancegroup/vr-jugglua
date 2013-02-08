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
	inline void reportRegistration(bool applicable) {
		std::cerr << (applicable ? "Registering " : "Skipping ") << getTypeName<T>() << " metamethod " << MetamethodName<Operator>::get() << " (" << MetamethodName<Operator>::getSymbol() << ")" << std::endl;
	}

	template<typename T, typename Operator>
	inline void pushAndSetOperator(lua_State * L, boost::mpl::true_ const&, boost::mpl::int_<2> const&) {
		reportRegistration<T, Operator>(true);
		//boost::mpl::for_each<osgTraits::math_and_arithmetic_types>(PrintInfoFunctor<T, Operator>());
		lua_pushcfunction(L, &(attemptBinaryOperator<Operator, T>));
		lua_setfield(L, -2, MetamethodName<Operator>::get());
	}

	template<typename T, typename Operator>
	inline void pushAndSetOperator(lua_State * L, boost::mpl::true_ const&, boost::mpl::int_<1> const&) {
		/* TESTING
		reportRegistration<T, Operator>(true);
		lua_pushcfunction(L, &(attemptUnaryOperator<Operator, T>));
		lua_setfield(L, -2, MetamethodName<Operator>::get());
		*/
	}

	template<typename T, typename Operator, typename Arity>
	inline void pushAndSetOperator(lua_State *, boost::mpl::false_ const&, Arity const&) {
		reportRegistration<T, Operator>(false);
	}

	struct RegistrationData {
		RegistrationData(lua_State * _L, introspection::Type const& t) : L(_L), metatableType(t), foundOurType(false) {}
		lua_State * L;
		introspection::Type const& metatableType;
		bool foundOurType;
	};

	template<typename T>
	struct visit_types {
		template<typename Operator>
		struct visit_operator {
			static void visit(RegistrationData const& d) {
				pushAndSetOperator<T, Operator>(d.L, typename osgTraits::is_operator_applicable<Operator, T>::type(), typename osgTraits::get_operator_arity<Operator>::type());
			}
		};

		static void visit(RegistrationData & d) {
			if (!d.foundOurType && introspection::Reflection::getType(extended_typeid<T>()) == d.metatableType) {
				d.foundOurType = true;
				boost::mpl::for_each<osgTraits::MathOperators, visit_operator<boost::mpl::_1> >(util::visitorState(d));
			}
		}
	};

	bool registerMathMetamethods(lua_State * L, introspection::Type const& t) {
		RegistrationData data(L, t);
		typedef boost::mpl::list<osg::Vec3d, osg::Vec3f, osg::Matrixd, osg::Quat> limitedTypes;
		boost::mpl::for_each<limitedTypes, visit_types<boost::mpl::_1> >(util::visitorState(data));
		return data.foundOurType;
	}

} // end of namespace osgLua

#endif
