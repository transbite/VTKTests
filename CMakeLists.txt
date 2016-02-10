project(VTKTests)

cmake_minimum_required(VERSION 2.8)

message("Config with cmake " ${CMAKE_VERSION})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)

if(Qt5Widgets_FOUND)
    message(STATUS "VTKTests - Qt5Widgets found...")
endif()

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
if(VTK_FOUND)
    message("VTK_VERSION:" ${VTK_VERSION})
endif()

if(UNIX)
##if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++0x)
##endif()
endif()

set(SRCS main.cpp mainwindow.cpp mainwindow.h 
	VtkTestWindow.cpp VtkTestWindow.h
	VtkTests.cpp VtkTests.h
	VtkTestsVolume.cpp
	GraphWidget.cpp GraphWidget.h
	)
set(FRMS mainwindow.ui GraphWidget.ui)

qt5_wrap_ui(FRMS_HDRS ${FRMS})

add_executable(VTKTests ${SRCS} ${FRMS_HDRS})

target_link_libraries(VTKTests ${VTK_LIBRARIES})

qt5_use_modules(VTKTests Widgets OpenGL Sql WebKitWidgets Concurrent)


