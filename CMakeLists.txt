project(VTKTests)

cmake_minimum_required(VERSION 2.8)

message("Config with cmake " ${CMAKE_VERSION})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)

if(Qt5Widgets_FOUND)
    message(STATUS "VTKTests - Qt5Widgets found...")
endif()

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(CTK REQUIRED)

if(CTK_FOUND)
    message("CTK package found in " ${CTK_DIR})
else()
    message("Could not find CTK package?")
endif()


set(PLUGIN_BUILD_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/plugins")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_BUILD_DIRECTORY})

add_subdirectory(SimpleQtPlugin)

add_subdirectory(SimpleCTKPlugin)

##message("VTK libraries:")
##foreach(lib ${VTK_LIBRARIES})
##    message("-" ${lib})
##endforeach(lib)
##message("VTK include directories: ")
##foreach(dir ${VTK_INCLUDE_DIRS})
##    message("-" ${dir})
##endforeach(dir)
##message("VTK use file: " ${VTK_USE_FILE})

##include_directories(${Qt5Widgets_Includes})
##include_directories(${Qt5OpenGL_Includes})

##message("1. cxx flags:")
##foreach(flag ${CMAKE_CXX_FLAGS})
##    message("- " ${flag})
##endforeach(flag)

##set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}" "-std=c++0x")

##set(CMAKE_CXX_FLAGS "-std=c++0x")

if(UNIX)
##if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++0x)
##endif()
endif()

##message("2. cxx flags:")
##foreach(flag ${CMAKE_CXX_FLAGS})
##    message("- " ${flag})
##endforeach(flag)

set(SRCS main.cpp mainwindow.cpp VtkTestWindow.cpp)
set(FRMS mainwindow.ui)

qt5_wrap_ui(FRMS_HDRS ${FRMS})

add_executable(VTKTests ${SRCS} ${FRMS_HDRS})

target_link_libraries(VTKTests ${VTK_LIBRARIES})

##target_link_libraries(VTKTests ${Qt5Widgets_LIBRARIES})
##target_link_libraries(VTKTests ${Qt5OpenGL_LIBRARIES})

qt5_use_modules(VTKTests Widgets OpenGL Sql WebKitWidgets)


