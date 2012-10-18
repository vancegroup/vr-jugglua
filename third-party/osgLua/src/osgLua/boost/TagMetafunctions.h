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
#ifndef INCLUDED_TagMetafunctions_h_GUID_b3581cba_1add_4518_b2ab_3bf71cb90454
#define INCLUDED_TagMetafunctions_h_GUID_b3581cba_1add_4518_b2ab_3bf71cb90454

// Internal Includes
#include "MathTypes.h"
#include "Tags.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/front.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/switch.hpp>

// Standard includes
// - none

namespace osgTraits {
	namespace detail {
		template<typename T>
		struct IsScalar : boost::is_arithmetic<T> {};
		/*
		typedef boost::mpl::list<
			boost::mpl::pair<boost::mpl::quote1<is_matrix>, tags::Matrix>,
			boost::mpl::pair<boost::mpl::quote1<is_vector>, tags::Vec>,
			boost::mpl::pair<boost::mpl::quote1<is_quat>, tags::Quat>,
			boost::mpl::pair<boost::mpl::quote1<IsScalar>, tags::Scalar> > CategoryTagMap;


		namespace impl {
			using namespace boost;
			using namespace boost::mpl::placeholders;
			template<typename T>
			struct ApplyFirst {
				template<typename P>
				struct apply {
					typedef typename mpl::first<P>::type key;
					typedef typename mpl::second<P>::type val;
					typedef typename mpl::apply<key, T>::type keyResult;
					typedef mpl::pair<keyResult, val> type;
				};
			};

			template<typename T>
			struct ApplyFirstIsTrueImpl {
				template<typename P>
				struct apply {
					typedef typename mpl::first<P>::type key;
					typedef typename key::template apply<T>::type type;
				};
			};
			template<typename T>
			struct ApplyFirstIsTrue {
				template<typename P>
				struct apply : ApplyFirstIsTrueImpl<T>::template apply<P>::type {};
			};

			template<typename T>
			struct filterFind {
				typedef typename mpl::filter_view<CategoryTagMap, mpl::lambda<typename ApplyFirstIsTrue<T>::template apply<_1> > >::type filtered;
				BOOST_MPL_ASSERT(( mpl::equal<mpl::size<filtered>, mpl::int_<1> > ));
				typedef typename mpl::front<filtered>::type front;
				typedef typename mpl::second<front>::type type;
			};
			template<typename T>
			struct findTag {
				typedef typename mpl::fold<CategoryTagMap,
					mpl::void_,
					mpl::if_< mpl::and_<mpl::apply<mpl::first<_2>, T>, mpl::equal<_1, mpl::void_> >, mpl::second<_2>, mpl::void_> >::type type;
			};
		}

		template<typename T>
		struct ComputeCategoryTag : impl::filterFind<T> {};
		*/
		/*
		template<typename T>
		struct ComputeCategoryTag : transform_view<CategoryTagMap, impl::ApplyFirst<T> > {};
		*/
		/*
		template<typename T>
		struct ComputeTag {

		};

		template<typename T>
		struct ComputeCategoryTag {
			typedef typename boost::mpl::if_<typename is_matrix<T>::type,
				tags::Matrix,
				boost::mpl::if_<typename is_vector<T>::type,
				tags::Vec

		};
		*/
		template<typename T, typename = void>
		struct ComputeCategoryTag {
			typedef void type;
		};

		template<typename T>
		struct ComputeCategoryTag<T, typename boost::enable_if<is_matrix<T> >::type> {
			typedef tags::Matrix type;
		};

		template<typename T>
		struct ComputeCategoryTag<T, typename boost::enable_if<is_vector<T> >::type> {
			typedef tags::Vec type;
		};

		template<typename T>
		struct ComputeCategoryTag<T, typename boost::enable_if<is_quat<T> >::type> {
			typedef tags::Quat type;
		};

		template<typename T>
		struct ComputeCategoryTag<T, typename boost::enable_if<IsScalar<T> >::type> {
			typedef tags::Scalar type;
		};
		/*
		template<typename T, typename = void>
		struct ComputeIsMathTag;

		template<typename T>
		struct ComputeIsMathTag<T, typename boost::enable_if<is_math_type<T> >::type> {
			typedef tags::MathType type;
		};

		template<typename T>
		struct ComputeIsMathTag<T, typename boost::enable_if<IsScalar<T> >::type> {
			typedef tags::Scalar type;
		};
		*/
		using boost::mpl::lambda;
		using boost::mpl::placeholders::_;
		using  boost::mpl::identity;
		typedef boost::mpl::list <
		boost::mpl::pair<is_math_type<_>, identity<tags::MathType> >,
		      boost::mpl::pair<IsScalar<_>, identity<tags::Scalar> >
		      > IsMathTagMap;

		template<typename T>
		struct ComputeIsMathTag : boost::mpl::switch_<IsMathTagMap, T> {};


	} // end of namespace detail
} // end of namespace osgTraits

#endif // INCLUDED_TagMetafunctions_h_GUID_b3581cba_1add_4518_b2ab_3bf71cb90454
