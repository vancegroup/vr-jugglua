# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC


include(${QT_USE_FILE})


set(SOURCES
	QTConsole.cpp)
	#QTConsoleView.cpp)
	
set(MOCS
	QTConsole.h)

set(API)
	#QTConsole.h)
	
set(UIS QTConsoleMainWindow.ui)

#set(RESOURCES QTConsoleResources.qrc)

# Let QT pre-process the files and add the generated files to the source list
qt4_wrap_ui(GENERATED_SOURCES ${UIS})
qt4_add_resources(GENERATED_SOURCES ${RESOURCES})
qt4_wrap_cpp(GENERATED_SOURCES ${MOCS})

source_group("Generated Sources - Do Not Edit" FILES ${GENERATED_SOURCES})

# Make sure the compiler can find the pre-processed files from qt4_wrap_ui
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(vrjugglua-qt
	${SOURCES}
	${API}
	${GENERATED_SOURCES}
	${UIS}
	${MOCS}
	${RESOURCES})
set_target_properties(vrjugglua-qt PROPERTIES PUBLIC_HEADER "${API}")
target_link_libraries(vrjugglua-qt
	vrjugglua
	${QT_LIBRARIES})
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjugglua-qt)
add_cppcheck(vrjugglua-qt)

set_property(TARGET
	vrjugglua-qt
	PROPERTY
	PROJECT_LABEL
	"VRJLua QT Console library")

if(INSTALL_CXX_SDK)
	install(TARGETS vrjugglua-qt
		EXPORT vrjlua-sdk
		RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		LIBRARY DESTINATION ${LIB_DIR} COMPONENT runtime
		ARCHIVE DESTINATION	${ARCH_DIR} COMPONENT sdk
		PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/vrjugglua/qt-console COMPONENT sdk)
endif()
