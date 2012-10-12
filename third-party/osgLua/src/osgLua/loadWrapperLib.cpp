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

#include <osgDB/DynamicLibrary>
#include <osgDB/Registry>
#include <osg/Version>

#include <vector>
#include <stdexcept>

#ifdef OSGLUA_VERBOSE
#include <iostream>
#endif



// borrowed from osgDB...
static std::string createLibraryNameForWrapper(const std::string& ext) {

#if defined(WIN32)
	// !! recheck evolving Cygwin DLL extension naming protocols !! NHV
#ifdef __CYGWIN__
	return "cygosgwrapper_" + ext + ".dll";
#elif defined(__MINGW32__)
	return "libosgwrapper_" + ext + ".dll";
#else
#ifdef _DEBUG
	return "osgwrapper_" + ext + "d.dll";
#else
	return "osgwrapper_" + ext + ".dll";
#endif
#endif
#elif macintosh
	return "osgwrapper_" + ext;
#elif defined(__hpux__)
	// why don't we use PLUGIN_EXT from the makefiles here?
	return "osgwrapper_" + ext + ".sl";
#else
	return "osgwrapper_" + ext + ".so";
#endif
}

namespace {

	static const char LIB[] = "lib";

	class LibNamePossibilityList {
		public:
			typedef std::vector<std::string> NameListType;
			typedef NameListType::iterator iterator;
			typedef NameListType::const_iterator const_iterator;

			const_iterator begin() const {
				return _names.begin();
			}

			iterator begin() {
				return _names.begin();
			}

			const_iterator end() const {
				return _names.end();
			}

			iterator end() {
				return _names.end();
			}

			void pushNameVariants(std::string const& basename) {
				pushDirectoryVariants(basename);
				if (basename.find(LIB) == 0) {
					// Starts with lib already - try stripping it. Starting at sizeof(LIB)
					// is one too far, due to null terminator.
					pushDirectoryVariants(basename.substr(sizeof(LIB) - 1, std::string::npos));
				} else {
					pushDirectoryVariants(LIB + basename);
				}
			}


		private:
			static std::string getPluginDirectoryPrepend() {
				static const std::string prepend = "osgPlugins-" + std::string(osgGetVersion()) + "/";
				return prepend;
			}
			void pushDirectoryVariants(std::string const& filename) {
				_names.push_back(filename);
				_names.push_back(getPluginDirectoryPrepend() + filename);
			}
			NameListType _names;
	};

} // end of anonymous namespace

osgDB::DynamicLibrary * loadWrapperLib(std::string const& libname) {
	/// We'll have a number of ideas
	LibNamePossibilityList names;

	/// First ask osgDB itself for its idea, and we'll stick lib in front of it as an extra measure.
	names.pushNameVariants(osgDB::Registry::instance()->createLibraryNameForNodeKit("osgwrapper_" + libname));

	/// Now use our copy/paste code to guess a name
	names.pushNameVariants(createLibraryNameForWrapper(libname));

	for (LibNamePossibilityList::const_iterator it = names.begin(), e = names.end(); it != e; ++it) {
		/// @todo replace the inside of this loop with osgDB::Registry::instance()->loadLibrary()
#ifdef OSGLUA_VERBOSE
		std::cout << "Trying to load " << *it << std::endl;
#endif
		osgDB::DynamicLibrary * lib = osgDB::DynamicLibrary::loadLibrary(*it);
		if (lib) {
#ifdef OSGLUA_VERBOSE
			std::cout << "Success!" << std::endl;
#endif
			return lib;
		}
	}

	throw std::runtime_error("Could not load wrapper for " + libname);

	return NULL;
}
