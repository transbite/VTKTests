PROJECT(Generated_CTK)

message(STATUS "GeneratedCTKPlugin project...")
message("GeneratedCTKPlugin build directory " ${PLUGIN_BUILD_DIRECTORY})
message("GeneratedCTKPlugin project binary directory " ${CMAKE_CURRENT_BINARY_DIR})

message("CTK_QT_VERSION = " ${CTK_QT_VERSION})

SET(PLUGIN_export_directive "Generated_CTK_EXPORT")

SET(PLUGIN_SRCS
  GeneratedCTKPlugin.cpp
)

# Files which should be processed by Qts moc
SET(PLUGIN_MOC_SRCS
  ##GeneratedCTKPlugin_p.h
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS
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
