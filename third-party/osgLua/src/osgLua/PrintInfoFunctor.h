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
#ifndef INCLUDED_PrintInfoFunctor_h_GUID_d31f32e7_cf9c_49fb_803e_08e52b032153
#define INCLUDED_PrintInfoFunctor_h_GUID_d31f32e7_cf9c_49fb_803e_08e52b032153

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none
namespace osgLua {
	template<typename T>
	inline std::string const& getTypeName() {
		return introspection::Reflection::getType(extended_typeid<T>()).getName();
	}

	template<typename T>
	inline const char * getIdentityTypeName() {
		return typeid(boost::mpl::identity<T>).name();
	}
	template<typename Operator, typename T1, typename T2>
	inline void printBinaryInfo() {
		std::cout << std::endl << getTypeName<T1>() << " and " << getTypeName<T2>() << ":" << std::endl;
		std::cout << "	HaveCompatibleScalar: " << osgTraits::BinaryPredicates::HaveCompatibleScalar<T1, T2>::type::value << std::endl;
		std::cout << "	HaveSameCategory: " << osgTraits::BinaryPredicates::HaveSameCategory<T1, T2>::type::value << std::endl;
		std::cout << "	HaveSameDimension: " << osgTraits::BinaryPredicates::HaveSameDimension<T1, T2>::type::value << std::endl;
		std::cout << "	CanTransformVecMatrix: " << osgTraits::BinaryPredicates::CanTransformVecMatrix<T1, T2>::type::value << std::endl;
		std::cout << "	AreVectorAndMatrix: " << osgTraits::BinaryPredicates::AreVectorAndMatrix<T1, T2>::type::value << std::endl;
		std::cout << "	HaveSameCategoryAndDimensionWithCompatibleScalar: " << osgTraits::BinaryPredicates::HaveSameCategoryAndDimensionWithCompatibleScalar<T1, T2>::type::value << std::endl;
		typedef typename boost::mpl::apply<Operator, T1, T2>::type SpecOp;
		typedef typename osgTraits::is_operator_available<SpecOp>::type IsAvail;
		std::cout << "	MultiplicationTag: " << getIdentityTypeName<typename osgTraits::MultiplicationTags::Compute<T1, T2>::type>() << std::endl;
		std::cout << "	SpecOp: " << typeid(SpecOp).name() << std::endl;
		std::cout << "	is_operator_available: " << IsAvail::value << std::endl;
		std::cout << "	get_operator_argument_c<T, 0>: " << getTypeName<typename osgTraits::get_operator_argument_c<SpecOp, 0>::type>() << std::endl;
		std::cout << "	get_operator_argument_c<T, 1>: " << getTypeName<typename osgTraits::get_operator_argument_c<SpecOp, 1>::type>() << std::endl;
	}

	template<typename T>
	inline void printInfo() {
		std::cout << std::endl << getTypeName<T>() << ":" << std::endl;
		std::cout << "	GetScalar: " << getTypeName<typename osgTraits::GetScalar<T>::type>() << std::endl;
		std::cout << "	GetDimension: " << osgTraits::GetDimension<T>::type::value << std::endl;
		std::cout << "	GetCategory: " << typeid(typename osgTraits::GetCategory<T>::type).name() << std::endl;

	}
	template<typename T1, typename Operator>
	struct PrintInfoFunctor {
		template<typename T2>
		void operator()(T2 const&) {
			printInfo<T2>();
			printBinaryInfo<Operator, T1, T2>();
		}
	};
} // end of namespace osgLua
#endif // INCLUDED_PrintInfoFunctor_h_GUID_d31f32e7_cf9c_49fb_803e_08e52b032153
