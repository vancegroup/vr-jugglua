/** @file
	@brief Header defining the base true and false trait types

	@date 2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

#pragma once
#ifndef INCLUDED_osgLuaBind_traitBase_h_GUID_23a04268_dfcd_4e25_94a2_381ce509ade7
#define INCLUDED_osgLuaBind_traitBase_h_GUID_23a04268_dfcd_4e25_94a2_381ce509ade7

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none


namespace osgLuaBind {

	/// Base class for "true" osg type traits.
	struct TraitTrue {
		static const bool value = true;

		static const bool trueIfTrue = true;
		static const bool falseIfTrue = false;

		typedef void trueType;
		typedef void type;
	};

	/// Base class for "false" osg type traits.
	struct TraitFalse {
		static const bool value = false;

		static const bool trueIfFalse = true;
		static const bool falseIfFalse = false;

		typedef void falseType;
	};

} // end of namespace osgLuaBind

#endif // INCLUDED_osgLuaBind_traitBase_h_GUID_23a04268_dfcd_4e25_94a2_381ce509ade7
