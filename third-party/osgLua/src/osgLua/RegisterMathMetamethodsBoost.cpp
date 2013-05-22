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
#include "UnaryOperatorDispatch.h"
#include "BinaryOperatorDispatch.h"
#include "OperatorMetamethodTraits.h"
#include "PrintInfoFunctor.h"
#include "StatefulTypeVisitFunctor.h"
#include <osgLua/LuaInclude>
#include <osgLua/Value>

#include <osgTraits/Operators.h>
#include <osgTraits/OperatorArity.h>
#include <osgTraits/IsOperatorAvailable.h>
#include <osgTraits/MathAndArithmeticTypes.h>

// Library/third-party includes
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/identity.hpp>

// Standard includes
// - none

namespace osgLua {
	template<typename T, typename Operator>
	inline void reportRegistration(bool applicable) {
		std::cerr << (applicable ? "Registering " : "Skipping ") << getTypeName<T>() << " metamethod " << MetamethodName<Operator>::get() << " (" << MetamethodName<Operator>::getSymbol() << ")" << std::flush << std::endl;
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
		struct visit_operators {
			static typename boost::enable_if< typename osgTraits::is_operator_applicable<Operator, T>::type>::type visit(RegistrationData const& d) {
				reportRegistration<T, Operator>(true);
				lua_pushcfunction(d.L, &(AttemptOperator<Operator, T>::attempt));
				lua_setfield(d.L, -2, MetamethodName<Operator>::get());
			}

			static void visit(...) {
				reportRegistration<T, Operator>(false);
			}
		};
		static void visit(RegistrationData & d) {
			std::cerr << "Trying " << getTypeName<T>() << std::flush << std::endl;
			if (!d.foundOurType && introspection::Reflection::getType(extended_typeid<T>()) == d.metatableType) {
				d.foundOurType = true;
				boost::mpl::for_each<osgTraits::MathOperators, visit_operators<boost::mpl::_1> >(util::visitorState(d));
			}
		}
	};

	bool registerMathMetamethods(lua_State * L, introspection::Type const& t) {
		RegistrationData data(L, t);
		std::cerr << "Attempting to register " << t.getQualifiedName() << std::flush << std::endl;
		boost::mpl::for_each<osgTraits::math_types, visit_types<boost::mpl::_1> >(util::visitorState(data));
		return data.foundOurType;
	}

} // end of namespace osgLua

#endif
