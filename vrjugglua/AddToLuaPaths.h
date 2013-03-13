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

	class RootDirectory : DirectoryBase<LuaPathTags::RootDirectory> {
		public:
			RootDirectory(std::string const& dir)
				: DirectoryBase<LuaPathTags::RootDirectory>(dir)	{}
	};

	class SearchDirectory : DirectoryBase<LuaPathTags::SearchDirectory> {
		public:
			SearchDirectory(std::string const& dir)
				: DirectoryBase<LuaPathTags::SearchDirectory>(dir)	{}
	};

	template<typename Tag>
	class SearchPathContainerBase {
		public:
			SearchPathContainerBase(SearchPath const& p)
				: _path(p) {}

			void insert(std::vector<std::string> const& patterns) {
				_path.insertAt(patterns, 1);
			}
			std::string toString() {
				return _path.toString();
			}
		private:
			SearchPath _path;
	};

	class LuaSearchPath : SearchPathContainerBase<LuaPathTags::LuaSearch> {
		public:
			LuaSearchPath(std::string const& s) : SearchPathContainerBase<LuaPathTags::LuaSearch>(s) {}
	};
	class LuaCSearchPath : SearchPathContainerBase<LuaPathTags::LuaCSearch> {
		public:
			LuaCSearchPath(std::string const& s) : SearchPathContainerBase<LuaPathTags::LuaCSearch>(s) {}
	};

	template<typename DirectoryTag, typename SearchTag>
	void extendLuaSearchPath(DirectoryBase<DirectoryTag> & d, SearchPathContainerBase<SearchTag> & s);

	/*
	class LuaSearchPattern {

	};

	class CatFirstTransform {
		public:
			CatFirstTransform(std::string const& s) : _s(s) {}
			template<typename T>
			std::string operator()(T const& a) {
				return _s + a;
			}
		private:
			std::string const _s;
	};

	class CatSecondTransform {
		public:
			CatSecondTransform(std::string const& s) : _s(s) {}
			template<typename T>
			std::string operator()(T const& a) {
				return a + _s;
			}
		private:
			std::string const _s;
	};


	class RootDirectory {
		public:
			RootDirectory(std::string const& dir)
				: _dir(ensureTrailingSlash(dir))
			{}

			typedef boost::array<const char *, 4> LuaDirSuffixesType;
			typedef boost::array<const char *, 2> LuaCDirSuffixesType;

			typedef boost::iterator_range<boost::transform_iterator<CatFirstTransform, LuaDirSuffixesType::iterator> > lua_dir_range_type;
			getLuaDirs() const;

			typedef boost::iterator_range<boost::transform_iterator<CatFirstTransform, LuaCDirSuffixesType::iterator> > lua_cdir_range_type;
			boost::iterator_range<boost::transform_iterator<CatFirstTransform, LuaCDirSuffixesType::iterator> >
			getLuaCDirs() const;
			std::string const& get() const {
				return _dir;
			}
		private:
			static const LuaDirSuffixesType s_luaDirs;
			static const LuaCDirSuffixesType s_luaCDirs;
			const std::string _dir;
	};

	*/
	void addRootToLuaPath(SearchPath & p, std::string const& root);

} // end of namespace vrjLua

#endif // INCLUDED_AddToLuaPaths_h_GUID_7c879366_eb17_457c_be5c_cd606e6943a2
