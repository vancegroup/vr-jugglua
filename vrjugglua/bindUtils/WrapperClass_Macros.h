/**	@file	WrapperClass_Macros.h
	@brief	header

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/
#pragma once
#ifndef INCLUDED_vrjugglua_bindUtils_WrapperClass_Macros_h
#define INCLUDED_vrjugglua_bindUtils_WrapperClass_Macros_h

// Local includes
// - none

// Library/third-party includes
#include <luabind/luabind.hpp>

#include <boost/preprocessor/cat.hpp>

// Standard includes
// - none

/**	Parameters:
#define BASECLASS LuaOsgApp
#define WRAPPERCLASS LuaOsgApp_wrapper
*/

#define STATIC_METHOD_NAME(NAME) BOOST_PP_CAT(default_, NAME)
#define METHOD_BIND(NAME)	.def(#NAME, &BASECLASS::NAME, &WRAPPERCLASS::STATIC_METHOD_NAME(NAME))
#define BEGIN_WRAPPER_CLASS struct WRAPPERCLASS : BASECLASS,  luabind::wrap_base

#define VOIDMETHOD_WRAP(NAME, PRE_CODE, POST_CODE)	\
	virtual void NAME() {						\
		PRE_CODE;									\
		call<void>(BOOST_PP_STRINGIZE(NAME));	\
		POST_CODE;									\
	}												\
	static void STATIC_METHOD_NAME(NAME)(BASECLASS * ptr) {		\
		return ptr->BASECLASS::NAME();								\
	}


/* Use like:

BEGIN_WRAPPER_CLASS {
	WRAPPERCLASS() {}
	METHOD_WRAP(void, initScene, , );
	METHOD_WRAP(void, preFrame, (BASECLASS::preFrame()), );
	METHOD_WRAP(void, latePreFrame, , (BASECLASS::latePreFrame()));
	METHOD_WRAP(void, intraFrame, , );
	METHOD_WRAP(void, postFrame, , );
};

void bindOsgAppToLua(LuaStatePtr state) {
	using namespace luabind;
	module(state.get(), "vrj") [
		class_<BASECLASS, WRAPPERCLASS>("OsgApp")
			.def(constructor<>())
			METHOD_BIND(initScene)
			METHOD_BIND(preFrame)
			METHOD_BIND(latePreFrame)
			METHOD_BIND(intraFrame)
			METHOD_BIND(postFrame)
	];
}
*/


#endif // INCLUDED_vrjugglua_bindUtils_WrapperClass_Macros_h
