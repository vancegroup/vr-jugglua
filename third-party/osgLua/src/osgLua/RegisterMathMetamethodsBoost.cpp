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

	template<typename T, typename Operator, typename Arity = typename osgTraits::get_arity<Operator>::type, typename = void>
	struct PushOperator {
		static void visit(RegistrationData const&) {		
			reportRegistration<T, Operator>(false);
		}
	};

	template<typename T, typename Operator>
	struct PushOperator<T, Operator, osgTraits::arity_tags::unary_tag, typename boost::enable_if< typename osgTraits::is_operator_applicable<Operator, T>::type>::type > {
		static void visit(RegistrationData const& d) {
			std::cerr << "pushing a unary operator" << std::endl;
			reportRegistration<T, Operator>(true);
			lua_pushcfunction(d.L, &(attemptUnaryOperator<Operator, T>));
			lua_setfield(d.L, -2, MetamethodName<Operator>::get());
		}
	};
	template<typename T, typename Operator>
	struct PushOperator<T, Operator, osgTraits::arity_tags::binary_tag, typename boost::enable_if< typename osgTraits::is_operator_applicable<Operator, T>::type>::type > {
		static void visit(RegistrationData const& d) {
			std::cerr << "pushing a binary operator" << std::endl;
			reportRegistration<T, Operator>(true);
			lua_pushcfunction(d.L, &(attemptBinaryOperator<Operator, T>));
			lua_setfield(d.L, -2, MetamethodName<Operator>::get());
		}
	};
	template<typename T>
	struct visit_types {

		static void visit(RegistrationData & d) {
			if (!d.foundOurType && introspection::Reflection::getType(extended_typeid<T>()) == d.metatableType) {
				d.foundOurType = true;
				boost::mpl::for_each<osgTraits::MathOperators, PushOperator<T, boost::mpl::_1> >(util::visitorState(d));
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
