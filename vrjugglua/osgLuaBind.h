/**	@file	osgLuaBind.h
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
#ifndef INCLUDED_vrjugglua_osgLuaBind_h
#define INCLUDED_vrjugglua_osgLuaBind_h

// Local includes
// - none

// Library/third-party includes
#include <luabind/luabind.hpp>

#include <osgLua/Value.h>

// Standard includes
// - none


//-- Reference Types --//

#define OSG_QUALIFIED_TYPENAME osg::Node
#include <osg/Node>
#include "osgLuaBind_refType.h"

#define OSG_QUALIFIED_TYPENAME osg::Group
#include <osg/Group>
#include "osgLuaBind_refType.h"

//-- Value Types --//

#define OSG_QUALIFIED_TYPENAME osg::Matrixd
#include <osg/Matrixd>
#include "osgLuaBind_valType.h"

#define OSG_QUALIFIED_TYPENAME osg::Vec3d
#include <osg/Vec3d>
#include "osgLuaBind_valType.h"

#define OSG_QUALIFIED_TYPENAME osg::Vec4d
#include <osg/Vec4d>
#include "osgLuaBind_valType.h"

#endif // INCLUDED_vrjugglua_osgLuaBind_h
