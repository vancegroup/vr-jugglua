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

//           Copyright Iowa State University 2013.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_ReplaceFirst_h_GUID_79D3CAE2_C92D_47B5_9CAF_85E958230250
#define INCLUDED_ReplaceFirst_h_GUID_79D3CAE2_C92D_47B5_9CAF_85E958230250


// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgTraits {
	namespace mp {
		using boost::mpl::fold;
		using boost::mpl::list0;
		using boost::mpl::pair;
		using boost::mpl::first;
		using boost::mpl::second;
		using boost::mpl::if_;
		using boost::mpl::_1;
		using boost::mpl::_2;
		using boost::mpl::push_back;
		using boost::mpl::copy;
		using boost::is_same;
		template<typename EmptyContainer, typename Inserter>
		struct replace_first {
			template<typename S, typename Needle, typename Replacement>
			struct apply {
				typedef typename fold < S,
				        pair<list0<>, Needle >,
				        if_ <
				        is_same<_2, second<_1> >,
				        pair< push_back<first<_1>, Replacement >, Needle >,
				        pair< push_back<first<_1>, _2 >, Replacement >
				        >
				        >::type fold_results;
				typedef typename copy< first<fold_results>, Inserter>::type type;
			};
		};
		typedef replace_first < vector0, back_inserter<vector0<> > vec_replace_first;
	} // end of namespace mp

} // end of namespace osgTraits

#endif // INCLUDED_ReplaceFirst_h_GUID_79D3CAE2_C92D_47B5_9CAF_85E958230250

