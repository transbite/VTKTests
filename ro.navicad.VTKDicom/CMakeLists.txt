PROJECT(ro_navicad_VTKDicom)

SET(PLUGIN_export_directive "ro_navicad_VTKDicom_EXPORT")

SET(PLUGIN_SRCS
  navicadVTKDicomPlugin.cpp
)

# Files which should be processed by Qts moc
SET(PLUGIN_MOC_SRCS
  ##navicadVTKDicomPlugin.h
	navicadVTKDicomPluginInterface.h
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS
	pluginInterfaceWidget.ui
)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
)

set(CTK_PLUGIN_LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_BUILD_DIRECTORY})


#Compute the plugin dependencies
ctkFunctionGetTargetLibraries(PLUGIN_target_libraries)

ctkMacroBuildPlugin(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${PLUGIN_export_directive}
  SRCS ${PLUGIN_SRCS}
  MOC_SRCS ${PLUGIN_MOC_SRCS}
  UI_FORMS ${PLUGIN_UI_FORMS}
  RESOURCES ${PLUGIN_resources}
  TARGET_LIBRARIES ${PLUGIN_target_libraries}
)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

qt5_use_modules(${PROJECT_NAME} Widgets)
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})
