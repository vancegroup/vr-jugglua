/**
	@file
	@brief Implementation

	@date 2013

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "AddToLuaPaths.h"

// Library/third-party includes
#include <boost/foreach.hpp>

// Standard includes
// - none

namespace vrjLua {
#define LUA_VER "5.1"

	namespace {
		template<typename T>
		class PushBackFunctor {
			public:
				PushBackFunctor(T & container) : _container(container) {}
				void operator()(typename T::value_type a) {
					_container.push_back(a);
				}
			private:
				T & _container;
		};
	} // end of namespace

	template<typename SearchTag>
	struct GetDirsFromRoot {
		static const char * dirs[];
	};

	template<>
	const char * GetDirsFromRoot<LuaPathTags::LuaSearch>::dirs[] = {
		"etc/vrjugglua/",
		"share/vrjugglua/lua/",
		"share/lua/" LUA_VER "/",
		"lib/lua/" LUA_VER "/"
	};

	template<>
	const char * GetDirsFromRoot<LuaPathTags::LuaCSearch>::dirs[] = {
		"etc/vrjugglua/",
		"share/vrjugglua/lua/",
		"share/lua/" LUA_VER "/",
		"lib/lua/" LUA_VER "/"
	};

	template<typename SearchTag>
	struct CallWithPatterns;

	template<>
	struct CallWithPatterns<LuaPathTags::LuaSearch> {
		template<typename F>
		static void apply(std::string const& dir, F func) {
			static const char * luaPatterns[] = {
				"?.lua",
				"?/init.lua"
			};
			BOOST_FOREACH(const char * patt, luaPatterns) {
				func(dir + patt);
			}
		}
	};

	template<>
	struct CallWithPatterns<LuaPathTags::LuaCSearch> {
		template<typename F>
		static void apply(std::string const& dir, F func) {
#ifdef _WIN32
			static const char luaCPattern[] = "?.dll";
#else
			static const char luaCPattern[] = "?.so";
#endif
			func(dir + luaCPattern);
		}
	};


	template<typename DirectoryTag, typename SearchTag>
	struct CallWithDirectory;

	template<typename SearchTag>
	struct CallWithDirectory<LuaPathTags::RootDirectory, SearchTag> {
		template<typename F>
		static void apply(std::string const& rootdir, F func) {
			BOOST_FOREACH(const char * patt, GetDirsFromRoot<SearchTag>::dirs) {
				CallWithPatterns<SearchTag>::apply(rootdir + patt, func);
			}
		}
	};

	template<typename SearchTag>
	struct CallWithDirectory<LuaPathTags::SearchDirectory, SearchTag> {
		template<typename F>
		static void apply(std::string const& rootdir, F func) {
			CallWithPatterns<SearchTag>::apply(rootdir, func);
		}
	};

	template<typename DirectoryTag, typename SearchTag>
	void extendLuaSearchPath(DirectoryBase<DirectoryTag> const& d, SearchPathContainerBase<SearchTag> & s) {
		typedef std::vector<std::string> StringList;
		StringList newpaths;
		CallWithDirectory<DirectoryTag, SearchTag>::apply(d.get(), PushBackFunctor<StringList>(newpaths));
		s.insert(newpaths);
	}

	template void extendLuaSearchPath(DirectoryBase<LuaPathTags::RootDirectory> const&, SearchPathContainerBase<LuaPathTags::LuaSearch> &);
	template void extendLuaSearchPath(DirectoryBase<LuaPathTags::SearchDirectory> const&, SearchPathContainerBase<LuaPathTags::LuaSearch> &);
	template void extendLuaSearchPath(DirectoryBase<LuaPathTags::RootDirectory> const&, SearchPathContainerBase<LuaPathTags::LuaCSearch> &);
	template void extendLuaSearchPath(DirectoryBase<LuaPathTags::SearchDirectory> const&, SearchPathContainerBase<LuaPathTags::LuaCSearch> &);

} // end of namespace vrjLua
