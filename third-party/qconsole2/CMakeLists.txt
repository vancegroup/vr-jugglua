# Embedded build for QConsole
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

include("${QT_USE_FILE}")
# Make sure the compiler can find the pre-processed files from qt4_wrap_cpp
include_directories(${CMAKE_BINARY_DIR})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

set(QCONSOLE_SRCS
	src/qconsole.cpp)

set(QCONSOLE_API
	include/q3richtext_p.h)

set(MOCS include/qconsole.h)
qt4_wrap_cpp(GENERATED_SOURCES ${MOCS})

source_group("Generated Sources - Do Not Edit" FILES ${GENERATED_SOURCES})

add_library(qconsole
	${QCONSOLE_SRCS}
	${QCONSOLE_API}
	${MOCS}
	${GENERATED_SOURCES})
target_link_libraries(qconsole ${QT_LIBRARIES})

set_property(TARGET
	qconsole
	PROPERTY
	PROJECT_LABEL
	"QConsole Widget")

set_target_properties(qconsole PROPERTIES PUBLIC_HEADER "${QCONSOLE_API};${MOCS}")

install(TARGETS qconsole
	EXPORT vrjlua-sdk
	ARCHIVE DESTINATION ${LIB_DIR}
	PUBLIC_HEADER DESTINATION ${INCLUDE_DIR})

set(QCONSOLE_LIBRARY qconsole PARENT_SCOPE)
set(QCONSOLE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)