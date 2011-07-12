/** @file
	@brief Header

	@date 2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

#pragma once
#ifndef INCLUDED_osgLuaBind_RefConverter_h_GUID_c42972ec_db46_44c0_8448_92bc76ca2bc0
#define INCLUDED_osgLuaBind_RefConverter_h_GUID_c42972ec_db46_44c0_8448_92bc76ca2bc0

// Internal Includes
#include "osgLuaBind_ConverterBase.h"
#include "osgLuaBind_luabindPassingStyle.h"
#include "osgLuaBind_isRefType.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgLuaBind {

	namespace detail {

		template<typename ValueType, typename ContainerType>
		struct RefConverter : ConverterBase<ValueType, ContainerType> {
			typedef ContainerType container_t;
			typedef ValueType * raw_ptr_t;
			/// @name Extra required methods for converter
			/// I think these are not provided by the native_converter_base in luabind because that
			/// library assumes that you won't make a converter for pointer types. These are minimal
			/// forwarding functions in any case.
			/// @{

			raw_ptr_t apply(lua_State* L, typename detail::luabindPassingStyle<container_t>::type &, int index) {
				return this->from(L, index);
			}

			void apply(lua_State* L, container_t const& value) {
				this->to(L, value);
			}

			int match(lua_State* L, typename detail::luabindPassingStyle<container_t>::type, int index) {
				return this->compute_score(L, index);
			}
			/// @}

		};
	} // end of namespace detail

} // end of namespace osgLuaBind


namespace luabind {
	template <typename T, typename Enable>
	struct default_converter;

	/// OSG referenced objects should be converted as such
	template <typename T>
	struct default_converter<T, typename osgLuaBind::IsRefType<T>::type>
			: osgLuaBind::detail::RefConverter<typename osgLuaBind::detail::unwrapPtr<T>::value_t, T> {};

} // end of namespace luabind
#endif // INCLUDED_osgLuaBind_RefConverter_h_GUID_c42972ec_db46_44c0_8448_92bc76ca2bc0
