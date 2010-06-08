# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(SOURCES
	)

set(API
	)

fltk_wrap_ui(vrjugglua-fltk vrjlua-fltk-console.fl)

add_library(vrjugglua-fltk ${SOURCES} ${API} ${vrjugglua-fltk_FLTK_UI_SRCS})
set_target_properties(vrjugglua-fltk PROPERTIES PUBLIC_HEADER "${API}")
target_link_libraries(vrjugglua-fltk vrjugglua ${FLTK_LIBRARIES})
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjugglua-fltk)
add_cppcheck(vrjugglua-fltk)

set_property(TARGET
	vrjugglua-fltk
	PROPERTY
	PROJECT_LABEL
	"VRJLua FLTK Console library")

install(TARGETS vrjugglua-fltk
	EXPORT vrjlua-sdk
	RUNTIME DESTINATION ${BIN_DIR}
	ARCHIVE DESTINATION	${LIB_DIR}
	PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/vrjugglua/fltk-console)

