project(SimpleQtPlugin)

message(STATUS "SimpleQtPlugin project...")
message("SimpleQtPlugin build directory " ${PLUGIN_BUILD_DIRECTORY})
message("SimpleQtPlugin project binary directory " ${CMAKE_CURRENT_BINARY_DIR})

if(Qt5Widgets_FOUND)
    message(STATUS "SimpleQtPlugin - Qt5Widgets found...")
else()
    message(STATUS "SimpleQtPlugin - Qt5Widgets NOT found!")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../")

set(PLUGIN_SRCS SimpleQtPlugin.cpp)

add_library(pnp_SimpleQtPlugin SHARED ${PLUGIN_SRCS})

qt5_use_modules(pnp_SimpleQtPlugin Widgets)
