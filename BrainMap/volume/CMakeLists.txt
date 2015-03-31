cmake_minimum_required(VERSION 2.8)

project(VolumeDemoApp)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

##find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
##include(${QT_USE_FILE})

find_package(Qt5Widgets)
find_package(Qt5Network)

include_directories("${PROJECT_BINARY_DIR}")

set(DemoSrcs main.cxx MainWindow.cxx)
set(DemoUI MainWindow.ui)
set(DemoHeaders MainWindow.h)

qt5_wrap_ui(DemoUISrcs ${DemoUI})
qt5_wrap_cpp(DemoMOCSrcs ${DemoHeaders})

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

add_executable(VolumeDemoApp ${DemoSrcs} ${DemoUISrcs} ${DemoMOCSrcs})
qt5_use_modules(VolumeDemoApp Widgets Network)
target_link_libraries(VolumeDemoApp ${VTK_LIBRARIES})

