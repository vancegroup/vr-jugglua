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
#include "SearchPath.h"

// Library/third-party includes
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>

#include <boost/assert.hpp>

// Standard includes
// - none

namespace vrjLua {

	SearchPath::SearchPath(std::string const& input, const char * delim) {
		boost::algorithm::split(_data, input, boost::algorithm::is_any_of(delim));
		_reapplyUniqueness();
	}

	void SearchPath::insertAt(StringList const& elts, size_t position) {
		BOOST_ASSERT(position <= _data.size() && "Position is bound by the size of the existing list!");
		_data.insert(_data.begin() + position, elts.begin(), elts.end());
		_reapplyUniqueness();
	}

	std::string SearchPath::toString(const char * delim) const {
		return boost::algorithm::join(_data, delim);
	}

	void SearchPath::_reapplyUniqueness() {
		StringList::iterator it = std::unique(_data.begin(), _data.end());
		_data.resize(std::distance(_data.begin(), it));
	}

}
