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

	namespace LuaPathTags {
		struct RootDirectory;
		struct SearchDirectory;
		struct LuaSearch;
		struct LuaCSearch;
	} // end of namespace LuaPathTags

	namespace detail {
		inline std::string ensureTrailingSlash(std::string dir) {
			if (dir.size() > 0) {
				const char back = *(dir.end() - 1);
				if (back != '/' && back != '\\') {
					dir += '/';
				}
			}
			return dir;
		}

		template<typename Tag>
		class DirectoryBase {
			public:
				DirectoryBase(std::string const& dir)
					: _dir(detail::ensureTrailingSlash(dir))
				{}
				std::string const& get() const {
					return _dir;
				}
			private:
				std::string const _dir;

		};

		template<typename Tag>
		class SearchPathContainerBase;

		template<typename DirectoryTag, typename SearchTag>
		extern void extendLuaSearchPath(DirectoryBase<DirectoryTag> const& d, SearchPathContainerBase<SearchTag> & s);

		template<typename Tag>
		class SearchPathContainerBase {
			public:
				SearchPathContainerBase(SearchPath const& p)
					: _path(p) {}

				void insert(std::vector<std::string> const& patterns) {
					_path.insertAt(patterns, 1);
				}

				template<typename DirectoryTag>
				void extend(DirectoryBase<DirectoryTag> const& d) {
					detail::extendLuaSearchPath(d, *this);
				}

				std::string toString() {
					return _path.toString();
				}
			private:
				SearchPath _path;
		};
	} // end of namespace detail




	class RootDirectory : public detail::DirectoryBase<LuaPathTags::RootDirectory> {
		public:
			RootDirectory(std::string const& dir)
				: detail::DirectoryBase<LuaPathTags::RootDirectory>(dir)	{}
	};

	class SearchDirectory : public detail::DirectoryBase<LuaPathTags::SearchDirectory> {
		public:
			SearchDirectory(std::string const& dir)
				: detail::DirectoryBase<LuaPathTags::SearchDirectory>(dir)	{}
	};



	class LuaSearchPath : public detail::SearchPathContainerBase<LuaPathTags::LuaSearch> {
		public:
			LuaSearchPath(std::string const& s) : detail::SearchPathContainerBase<LuaPathTags::LuaSearch>(s) {}
	};

	class LuaCSearchPath : public detail::SearchPathContainerBase<LuaPathTags::LuaCSearch> {
		public:
			LuaCSearchPath(std::string const& s) : detail::SearchPathContainerBase<LuaPathTags::LuaCSearch>(s) {}
	};



} // end of namespace vrjLua

#endif // INCLUDED_AddToLuaPaths_h_GUID_7c879366_eb17_457c_be5c_cd606e6943a2
