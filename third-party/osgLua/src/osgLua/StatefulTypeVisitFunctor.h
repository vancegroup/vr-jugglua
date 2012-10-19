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
#ifndef INCLUDED_StatefulTypeVisitFunctor_h_GUID_f0081382_2f43_4f21_8a0a_e7dc8e5ac052
#define INCLUDED_StatefulTypeVisitFunctor_h_GUID_f0081382_2f43_4f21_8a0a_e7dc8e5ac052

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none

namespace util {
	template<typename StateClass>
	class StatefulTypeVisitFunctor {
		public:
			typedef typename boost::add_reference<StateClass>::type StateRef;
			StatefulTypeVisitFunctor(StateRef & d) : data(d) {}

			template<class Visitor>
			void operator()(Visitor const&) {
				Visitor::visit(data);
			}
		private:
			StateRef & data;
	};

	template<typename StateClass>
	StatefulTypeVisitFunctor<StateClass> visitorState(StateClass & data) {
		return	StatefulTypeVisitFunctor<StateClass>(data);
	}
} // end of namespace util
#endif // INCLUDED_StatefulTypeVisitFunctor_h_GUID_f0081382_2f43_4f21_8a0a_e7dc8e5ac052
