project(VTKTests)

cmake_minimum_required(VERSION 3.0)

message("Config with cmake " ${CMAKE_VERSION})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

message("VTK libraries:")
foreach(lib ${VTK_LIBRARIES})
    message("-" ${lib})
endforeach(lib)
message("VTK include directories: ")
foreach(dir ${VTK_INCLUDE_DIRS})
    message("-" ${dir})
endforeach(dir)
message("VTK use file: " ${VTK_USE_FILE})

##include_directories(${Qt5Widgets_Includes})
##include_directories(${Qt5OpenGL_Includes})

##message("1. cxx flags:")
##foreach(flag ${CMAKE_CXX_FLAGS})
##    message("- " ${flag})
##endforeach(flag)

##set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}" "-std=c++0x")

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

qt5_use_modules(VTKTests Widgets OpenGL)


