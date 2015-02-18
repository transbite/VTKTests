project(SimpleQtPlugin)

message(STATUS "SimpleQtPlugin project...")

if(Qt5Widgets_FOUND)
    message(STATUS "SimpleQtPlugin - Qt5Widgets found...")
else()
    message(STATUS "SimpleQtPlugin - Qt5Widgets NOT found!")
endif()

set(PLUGIN_SRCS SimpleQtPlugin.cpp)

add_library(SimpleQtPlugin ${PLUGIN_SRCS})

qt5_use_modules(SimpleQtPlugin Widgets)
