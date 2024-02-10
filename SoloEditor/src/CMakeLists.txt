set(EDITOR_SOURCES
		"EditorLayer.cpp"
		"EditorLayer.h"
		"SoloEditorApp.cpp"

)

file(GLOB_RECURSE GLSL_SOURCE_FILES "../assets/shaders/*.glsl")

source_group(Shaders FILES ${GLSL_SOURCE_FILES})

add_executable(SoloEditor
		${EDITOR_SOURCES}
		${GLSL_SOURCE_FILES}
)

add_custom_command(TARGET SoloEditor PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/../res "${CMAKE_SOURCE_DIR}/bin/Release/SoloEditor/res")

add_custom_command(TARGET SoloEditor PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/../res "${CMAKE_SOURCE_DIR}/bin/Debug/SoloEditor/res")


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
			FILES ${EDITOR_SOURCES}
)
