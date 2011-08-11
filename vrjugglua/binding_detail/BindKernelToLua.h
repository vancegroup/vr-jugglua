/**	@file	binding_detail/BindKernelToLua.h
	@brief	header exposing binding function for vrjKernel and standard
			components

	@date
	2009-2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2009-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_vrjugglua_binding_detail_BindKernelToLua_h
#define INCLUDED_vrjugglua_binding_detail_BindKernelToLua_h

// Local includes
#include <vrjugglua/LuaScript.h>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace vrjLua {

	void bindKernelToLua(LuaStatePtr state);

/// @brief Class for internal use - use the static methods in LuaScript.h
	class KernelState {
		public:
			static void init(boost::program_options::variables_map vm);
			static void init(int argc, char* argv[]);
			static void init();
			static void initAsSingleMachine();
			static void initAsClusterPrimaryNode();
			static void initAsClusterSecondaryNode(int port = 0);
			static bool hasInitialized();
			static boost::program_options::options_description getVrjOptionsDescriptions();

		protected:
			static bool _init;
			static bool _isCluster;
			static bool _isPrimary;
	};

} // end of vrjLua namespace



#endif // INCLUDED_vrjugglua_binding_detail_BindKernelToLua_h
