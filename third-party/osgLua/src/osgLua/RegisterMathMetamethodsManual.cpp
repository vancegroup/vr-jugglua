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


#ifndef OSGLUA_HAVE_BOOST

// Internal Includes
#include "RegisterMathMetamethods.h"
#include "VectorTemplates.h"
#include "MatrixTemplates.h"

// Library/third-party includes

// cd /usr/include && find osg -name "Vec*" | sort | sed 's/\(.*\)/\#include \<\1\>/g'
#include <osg/Vec2>
#include <osg/Vec2b>
#include <osg/Vec2d>
#include <osg/Vec2f>
#include <osg/Vec2s>
#include <osg/Vec3>
#include <osg/Vec3b>
#include <osg/Vec3d>
#include <osg/Vec3f>
#include <osg/Vec3s>
#include <osg/Vec4>
#include <osg/Vec4b>
#include <osg/Vec4d>
#include <osg/Vec4f>
#include <osg/Vec4s>
#include <osg/Vec4ub>

#include <osg/Matrix>
#include <osg/Matrixd>
#include <osg/Matrixf>

// Standard includes
// - none

namespace osgLua {
	bool registerMathMetamethods(lua_State * L, introspection::Type const& t) {
		/// Bind mathematically-inclined values specially - only
		/// one such binding will succeed for a type, obviously, so
		/// exit the method immediately at that point.

#define BIND_VECTOR(TYPE) \
		if (Vector::bind_metamethods<TYPE>(L, t)) { \
	  		return true; \
	  	}

		BIND_VECTOR(osg::Vec4d)
		BIND_VECTOR(osg::Vec4f)
		BIND_VECTOR(osg::Vec4b)
		BIND_VECTOR(osg::Vec4ub)
		BIND_VECTOR(osg::Vec4s)
		BIND_VECTOR(osg::Vec4)
		BIND_VECTOR(osg::Vec3d)
		BIND_VECTOR(osg::Vec3f)
		BIND_VECTOR(osg::Vec3s)
		BIND_VECTOR(osg::Vec3b)
		BIND_VECTOR(osg::Vec3)
		BIND_VECTOR(osg::Vec2d)
		BIND_VECTOR(osg::Vec2f)
		BIND_VECTOR(osg::Vec2b)
		BIND_VECTOR(osg::Vec2s)
		BIND_VECTOR(osg::Vec2)

#undef BIND_VECTOR


#define BIND_MATRIX(TYPE) \
		if (Matrix::bind_metamethods<TYPE>(L, t)) { \
	  		return true; \
	  	}

		BIND_MATRIX(osg::Matrixd)
		BIND_MATRIX(osg::Matrixf)
		BIND_MATRIX(osg::Matrix)

#undef BIND_MATRIX

		return false;
	}

} // end of namespace osgLua

#endif
