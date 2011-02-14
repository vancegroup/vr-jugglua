/*
	osgLua: use Lua to access dynamically to osg using osgIntrospection
	Copyright(C) 2006 Jose L. Hidalgo Valiño (PpluX) (pplux at pplux.com)
	Copyright(C) 2010-2011 Iowa State University (Author: Ryan Pavlik <rpavlik@acm.org> )

    This library is open source and may be redistributed and/or modified under
    the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
    (at your option) any later version.  The full license is in LICENSE file
    included with this distribution, and on the openscenegraph.org website.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    OpenSceneGraph Public License for more details.
*/

#include "loadWrapperLib.h"

#include <osg/Version>

#include <vector>
#include <stdexcept>
#include <iostream>

std::string getLibraryNamePrepend() {
	return std::string("osgPlugins-")+std::string(osgGetVersion())+std::string("/");
}

// borrowed from osgDB...
std::string createLibraryNameForWrapper(const std::string& ext) {

#if defined(WIN32)
	// !! recheck evolving Cygwin DLL extension naming protocols !! NHV
#ifdef __CYGWIN__
	return "cygosgwrapper_"+ext+".dll";
#elif defined(__MINGW32__)
	return "libosgwrapper_"+ext+".dll";
#else
#ifdef _DEBUG
	return "osgwrapper_"+ext+"d.dll";
#else
	return "osgwrapper_"+ext+".dll";
#endif
#endif
#elif macintosh
	return "osgwrapper_"+ext;
#elif defined(__hpux__)
	// why don't we use PLUGIN_EXT from the makefiles here?
	return "osgwrapper_"+ext+".sl";
#else
	return "osgwrapper_"+ext+".so";
#endif
}

osgDB::DynamicLibrary * loadWrapperLib(std::string const& libname) {
    std::string basename = createLibraryNameForWrapper(libname);
	std::vector<std::string> libnames;
	libnames.push_back(basename);
	libnames.push_back("lib"+basename);
	std::string const libdir = getLibraryNamePrepend();
	unsigned int n = libnames.size();
	for (unsigned int i = 0; i < n; ++i) {
		libnames.push_back(libdir + libnames[i]);
	}
	
	n = libnames.size();
	for (unsigned int i = 0; i < n; ++i) {
		std::cout << "Trying to load " << libnames[i] << std::endl;
		osgDB::DynamicLibrary * lib = osgDB::DynamicLibrary::loadLibrary(libnames[i]);
		if (lib) {
			std::cout << "Success!" << std::endl;
			return lib;
		}
	}
	
    throw std::runtime_error("Could not load wrapper for " + libname);
	
	return NULL;
}
