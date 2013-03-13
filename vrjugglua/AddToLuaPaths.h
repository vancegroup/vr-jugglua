/** @file
	@brief Header

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

#pragma once
#ifndef INCLUDED_AddToLuaPaths_h_GUID_7c879366_eb17_457c_be5c_cd606e6943a2
#define INCLUDED_AddToLuaPaths_h_GUID_7c879366_eb17_457c_be5c_cd606e6943a2

// Internal Includes
#include "SearchPath.h"

// Library/third-party includes
#include <boost/array.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/transform_iterator.hpp>

// Standard includes
// - none


namespace vrjLua {



	inline std::string ensureTrailingSlash(std::string dir) {
		if (dir.size() > 0) {
			const char back = *(dir.end() - 1);
			if (back != '/' && back != '\\') {
				dir += '/';
			}
		}
		return dir;
	}

	namespace LuaPathTags {
		struct RootDirectory;
		struct SearchDirectory;
		struct LuaSearch;
		struct LuaCSearch;
	};

	template<typename Tag>
	class DirectoryBase {
		public:
			DirectoryBase(std::string const& dir)
				: _dir(ensureTrailingSlash(dir))
			{}
			std::string const& get() const {
				return _dir;
			}
		private:
			std::string const _dir;

	};

	class RootDirectory : public DirectoryBase<LuaPathTags::RootDirectory> {
		public:
			RootDirectory(std::string const& dir)
				: DirectoryBase<LuaPathTags::RootDirectory>(dir)	{}
	};

	class SearchDirectory : public DirectoryBase<LuaPathTags::SearchDirectory> {
		public:
			SearchDirectory(std::string const& dir)
				: DirectoryBase<LuaPathTags::SearchDirectory>(dir)	{}
	};

	template<typename Tag>
	class SearchPathContainerBase {
		public:
			SearchPathContainerBase(SearchPath const& p)
				: _path(p) {}

			SearchPathContainerBase()
				: _path() {}

			void set(std::string const& s) {
				_path.set(s);
			}

			void insert(std::vector<std::string> const& patterns) {
				_path.insertAt(patterns, 1);
			}

			std::string toString() {
				return _path.toString();
			}
		private:
			SearchPath _path;
	};

	class LuaSearchPath : public SearchPathContainerBase<LuaPathTags::LuaSearch> {
		public:
			LuaSearchPath(std::string const& s) : SearchPathContainerBase<LuaPathTags::LuaSearch>(s) {}
			LuaSearchPath() : SearchPathContainerBase<LuaPathTags::LuaSearch>() {}
	};

	class LuaCSearchPath : public SearchPathContainerBase<LuaPathTags::LuaCSearch> {
		public:
			LuaCSearchPath(std::string const& s) : SearchPathContainerBase<LuaPathTags::LuaCSearch>(s) {}
			LuaCSearchPath() : SearchPathContainerBase<LuaPathTags::LuaCSearch>() {}
	};

	template<typename DirectoryTag, typename SearchTag>
	extern void extendLuaSearchPath(DirectoryBase<DirectoryTag> const& d, SearchPathContainerBase<SearchTag> & s);


} // end of namespace vrjLua

#endif // INCLUDED_AddToLuaPaths_h_GUID_7c879366_eb17_457c_be5c_cd606e6943a2
