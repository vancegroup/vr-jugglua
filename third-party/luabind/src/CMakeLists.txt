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

set(LUABIND_EXTRA_LIBS)
if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
elseif(APPLE)

else() # Linux
	list(APPEND LUABIND_EXTRA_LIBS m)
endif()

add_library(luabind ${LUABIND_SRCS})
target_link_libraries(luabind lua ${LUABIND_EXTRA_LIBS})
