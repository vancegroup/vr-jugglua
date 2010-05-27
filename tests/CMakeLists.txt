# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(TESTS_DATA "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(LUA_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/luatests")

foreach(script stop-kernel minimal-osg initialize-devices osgapp-delegate) # inherit-osgapp
    add_test(NAME
        vrjlua-run-${script}_lua
        COMMAND
        vrjlua
        "${LUA_TESTS}/${script}.lua")
endforeach()

#foreach(script osgapp-delegate)
#    add_test(NAME
#        vrjlua-delegation-run-${script}_lua
#        COMMAND
#        vrjlua-delegation
#        "${LUA_TESTS}/${script}.lua")
#endforeach()
