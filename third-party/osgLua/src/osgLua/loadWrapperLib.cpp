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
#include <osgDB/FileUtils>
#include <osg/Version>
#include <osg/Notify>

#include <vector>
#include <stdexcept>
#include <algorithm>

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
				OSG_INFO << "Given library basename " << basename << std::endl;
				pushDirectoryVariants(basename);
				if (basename.find(LIB) == 0) {
					// Starts with lib already - try stripping it. Starting at sizeof(LIB)
					// is one too far, due to null terminator.
					const std::string noLib = basename.substr(sizeof(LIB) - 1, std::string::npos);
					OSG_INFO << "Varying basename by stripping " << LIB << ": " << noLib << std::endl;
					pushDirectoryVariants(noLib);
				} else {
					OSG_INFO << "Varying basename by adding a prefix: " << LIB << basename << std::endl;
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
				const std::string withDir = getPluginDirectoryPrepend() + filename;
				OSG_INFO << "Adding directory variant " << withDir << std::endl;
				_names.push_back(withDir);
			}
			NameListType _names;
	};

	struct OsgInfoPathListPrinter {
		void operator()(std::string const& str) {
			if (str.empty()) {
				OSG_INFO << "[EmptyEntry!]";
			} else {
				OSG_INFO << str;
			}
			OSG_INFO << ";";
		}
	};

} // end of anonymous namespace

osgDB::DynamicLibrary * loadWrapperLib(std::string const& libname) {
	/// We'll have a number of ideas
	LibNamePossibilityList names;

	/// First ask osgDB itself for its idea, and we'll stick lib in front of it as an extra measure.
	OSG_INFO << std::endl << "Asking osgDB::Registry what it would call the wrapper library for " << libname << std::endl;
	names.pushNameVariants(osgDB::Registry::instance()->createLibraryNameForNodeKit("osgwrapper_" + libname));

	/// Now use our copy/paste code to guess a name
	OSG_INFO << std::endl << "Using our own code to compute a (possibly redundant) guess." << std::endl;
	names.pushNameVariants(createLibraryNameForWrapper(libname));

	for (LibNamePossibilityList::const_iterator it = names.begin(), e = names.end(); it != e; ++it) {
		/// @todo replace the inside of this loop with osgDB::Registry::instance()->loadLibrary()
		/// This would let OSG manage library lifetimes for us - a mixed blessing, since it does it
		/// globally, but probably better than right now, where we'll joyfully crash if you
		/// load the same lib twice.
		OSG_INFO << "Trying to load " << *it << std::endl;

		osgDB::DynamicLibrary * lib = osgDB::DynamicLibrary::loadLibrary(*it);
		if (lib) {
			OSG_INFO << "Success!" << std::endl;
			return lib;
		}
	}

	throw std::runtime_error("Could not load wrapper for " + libname);

	return NULL;
}


void outputLibraryPathListToOsgInfo() {
	if (osg::isNotifyEnabled(osg::INFO)) {
		OSG_INFO << "[OSGLUA] osgLua loaded! Here is the library path list:";
		osgDB::FilePathList & lpl = osgDB::getLibraryFilePathList();
		std::for_each(lpl.begin(), lpl.end(), OsgInfoPathListPrinter());
		OSG_INFO << std::endl;
	}
}
