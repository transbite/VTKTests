project(SimpleCTKPlugin)

find_package(Qt5Concurrent REQUIRED)

message(STATUS "SimpleCTKPlugin project...")
message("SimpleCTKPlugin build directory " ${PLUGIN_BUILD_DIRECTORY})
message("SimpleCTKPlugin project binary directory " ${CMAKE_CURRENT_BINARY_DIR})

#message("CTK libraries")
#foreach(ctklib ${CTK_LIBRARIES})
#    message("-" ${ctklib})
#endforeach(ctklib)

#message("CTK include directories")
#foreach(ctkdir ${CTK_INCLUDE_DIRS})
#    message("-" ${ctkdir})
#endforeach(ctkdir)

include_directories(${CTK_INCLUDE_DIRS})

set(CTK_PLUGIN_SRC SimpleCTKPlugin.cpp)

add_library(pnp_SimpleCTKPlugin SHARED ${CTK_PLUGIN_SRC})

target_link_libraries(pnp_SimpleCTKPlugin CTKPluginFramework)

qt5_use_modules(pnp_SimpleCTKPlugin Concurrent)
