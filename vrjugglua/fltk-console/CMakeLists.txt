# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(SOURCES
	FLTKConsole.cpp
	vrjlua-fltk-console.fl)

set(API
	FLTKConsole.h)

fltk_wrap_ui(vrjugglua-fltk vrjlua-fltk-console.fl)
source_group("FLTK Generated Files - Do Not Edit" FILES ${vrjugglua-fltk_FLTK_UI_SRCS})

add_library(vrjugglua-fltk ${SOURCES} ${API} ${vrjugglua-fltk_FLTK_UI_SRCS})
set_target_properties(vrjugglua-fltk PROPERTIES PUBLIC_HEADER "${API}")
target_link_libraries(vrjugglua-fltk vrjugglua ${FLTK_GUI_LIBS})
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjugglua-fltk)
add_cppcheck(vrjugglua-fltk)

set_property(TARGET
	vrjugglua-fltk
	PROPERTY
	PROJECT_LABEL
	"VRJLua FLTK Console library")

if(INSTALL_CXX_SDK)
	install(TARGETS vrjugglua-fltk
		EXPORT vrjlua-sdk
		RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		LIBRARY DESTINATION ${LIB_DIR} COMPONENT runtime
		ARCHIVE DESTINATION	${ARCH_DIR} COMPONENT sdk
		PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/vrjugglua/fltk-console COMPONENT sdk)
endif()
