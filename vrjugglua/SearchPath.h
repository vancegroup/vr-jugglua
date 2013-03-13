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
#ifndef INCLUDED_SearchPath_h_GUID_1091ef29_04ea_4776_a191_3d2bf5a56ecd
#define INCLUDED_SearchPath_h_GUID_1091ef29_04ea_4776_a191_3d2bf5a56ecd

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#include <string>
#include <vector>

namespace vrjLua {
	class SearchPath {
		public:
			static const char * defaultDelimiter() {
				return ";";
			}

			typedef std::vector<std::string> StringList;

			SearchPath() {}
			SearchPath(std::string const& input, const char * delim = defaultDelimiter());

			void set(std::string const& input, const char * delim = defaultDelimiter());

			void insertAt(StringList const& elts, size_t position = 0);

			std::string toString(const char * delim = defaultDelimiter()) const;
		private:
			StringList _data;

			void _reapplyUniqueness();
	};


}

#endif // INCLUDED_SearchPath_h_GUID_1091ef29_04ea_4776_a191_3d2bf5a56ecd
