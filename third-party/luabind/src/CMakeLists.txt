# Embedded build for LuaBind
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(LUABIND_SRCS
	class.cpp
	class_info.cpp
	class_registry.cpp
	class_rep.cpp
	create_class.cpp
	error.cpp
	exception_handler.cpp
	function.cpp
	inheritance.cpp
	link_compatibility.cpp
	object_rep.cpp
	open.cpp
	pcall.cpp
	scope.cpp
	stack_content_by_name.cpp
	weak_ref.cpp
	wrapper_base.cpp)

set(LUABIND_API
	../luabind/value_wrapper.hpp
	../luabind/weak_ref.hpp
	../luabind/luabind.hpp
	../luabind/raw_policy.hpp
	../luabind/class.hpp
	../luabind/class_info.hpp
	../luabind/adopt_policy.hpp
	../luabind/out_value_policy.hpp
	../luabind/handle.hpp
	../luabind/prefix.hpp
	../luabind/function.hpp
	../luabind/get_pointer.hpp
	../luabind/copy_policy.hpp
	../luabind/discard_result_policy.hpp
	../luabind/back_reference_fwd.hpp
	../luabind/open.hpp
	../luabind/config.hpp
	../luabind/tag_function.hpp
	../luabind/get_main_thread.hpp
	../luabind/lua_include.hpp
	../luabind/make_function.hpp
	../luabind/iterator_policy.hpp
	../luabind/dependency_policy.hpp
	../luabind/exception_handler.hpp
	../luabind/version.hpp
	../luabind/return_reference_to_policy.hpp
	../luabind/back_reference.hpp
	../luabind/yield_policy.hpp)

set(LUABIND_DETAIL_API
	../luabind/detail/call_function.hpp
	../luabind/detail/signature_match.hpp
	../luabind/detail/conversion_storage.hpp
	../luabind/detail/garbage_collector.hpp
	../luabind/detail/object_rep.hpp
	../luabind/detail/inheritance.hpp
	../luabind/detail/most_derived.hpp
	../luabind/detail/typetraits.hpp
	../luabind/detail/pointee_typeid.hpp
	../luabind/detail/convert_to_lua.hpp
	../luabind/detail/stack_utils.hpp
	../luabind/detail/property.hpp
	../luabind/detail/class_registry.hpp
	../luabind/detail/open.hpp
	../luabind/detail/pcall.hpp
	../luabind/detail/class_rep.hpp
	../luabind/detail/call_member.hpp
	../luabind/detail/enum_maker.hpp
	../luabind/detail/deduce_signature.hpp
	../luabind/detail/format_signature.hpp
	../luabind/detail/call.hpp
	../luabind/detail/operator_id.hpp
	../luabind/detail/other.hpp
	../luabind/detail/instance_holder.hpp
	../luabind/detail/pointee_sizeof.hpp
	../luabind/detail/policy.hpp
	../luabind/detail/has_get_pointer.hpp
	../luabind/detail/link_compatibility.hpp
	../luabind/detail/yes_no.hpp
	../luabind/detail/make_instance.hpp
	../luabind/detail/constructor.hpp
	../luabind/detail/object_call.hpp
	../luabind/detail/ref.hpp
	../luabind/detail/decorate_type.hpp
	../luabind/detail/call_operator_iterate.hpp
	../luabind/detail/get_signature.hpp
	../luabind/detail/debug.hpp
	../luabind/detail/primitives.hpp
	../luabind/wrapper_base.hpp
	../luabind/container_policy.hpp
	../luabind/object.hpp
	../luabind/scope.hpp
	../luabind/error.hpp
	../luabind/operator.hpp
	../luabind/nil.hpp
	../luabind/typeid.hpp
	../luabind/shared_ptr_converter.hpp
	../luabind/from_stack.hpp)

set(LUABIND_EXTRA_LIBS)
if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
elseif(APPLE)

else() # Linux
	list(APPEND LUABIND_EXTRA_LIBS m)
endif()

add_library(luabind ${LUABIND_SRCS})
target_link_libraries(luabind lua ${LUABIND_EXTRA_LIBS})

set_property(TARGET
	luabind
	PROPERTY
	PROJECT_LABEL
	"Third-Party: LuaBind Library")

if(INSTALL_CXX_SDK)
	install(FILES ${LUABIND_API}
		DESTINATION ${INCLUDE_DIR}/luabind)
	install(FILES ${LUABIND_DETAIL_API}
		DESTINATION ${INCLUDE_DIR}/luabind/detail)
	install(TARGETS luabind
		EXPORT vrjlua-sdk
		ARCHIVE DESTINATION ${LIB_DIR})
endif()
