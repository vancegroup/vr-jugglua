/** @file
	@brief Header

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

#pragma once
#ifndef INCLUDED_CopyToFlatSequence_h_GUID_5a903eb1_ca6d_422f_bf19_e36ad42de6a2
#define INCLUDED_CopyToFlatSequence_h_GUID_5a903eb1_ca6d_422f_bf19_e36ad42de6a2

// Internal Includes
// - none

// Library/third-party includes
#include <boost/mpl/copy.hpp>
#include <boost/mpl/front_inserter.hpp>
#include <boost/mpl/list.hpp>

// Standard includes
// - none


namespace osgTraits {

	namespace detail {
		typedef boost::mpl::front_inserter<boost::mpl::list0<> > SequenceInserter;

		template<typename Sequence>
		struct copy_to_flat_sequence : boost::mpl::copy<Sequence, SequenceInserter> {};
	} // end of namespace detail

} // end of namespace osgTraits

#endif // INCLUDED_CopyToFlatSequence_h_GUID_5a903eb1_ca6d_422f_bf19_e36ad42de6a2
