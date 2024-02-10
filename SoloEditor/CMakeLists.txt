include(CMakePrintHelpers)
include(CheckIPOSupported)

project(SoloEditor VERSION 0.0.1 LANGUAGES CXX)


add_subdirectory(src)

set_target_properties(SoloEditor PROPERTIES
					ARCHIVE_OUTPUT_DIRECTORY_DEBUG		${CMAKE_SOURCE_DIR}/bin/Debug/SoloEditor
					ARCHIVE_OUTPUT_DIRECTORY_RELEASE	${CMAKE_SOURCE_DIR}/bin/Release/SoloEditor
					ARCHIVE_OUTPUT_DIRECTORY_DIST		${CMAKE_SOURCE_DIR}/bin/Dist/SoloEditor
					LIBRARY_OUTPUT_DIRECTORY_DEBUG		${CMAKE_SOURCE_DIR}/bin/Debug/SoloEditor
					LIBRARY_OUTPUT_DIRECTORY_RELEASE	${CMAKE_SOURCE_DIR}/bin/Release/SoloEditor
					LIBRARY_OUTPUT_DIRECTORY_DIST		${CMAKE_SOURCE_DIR}/bin/Dist/SoloEditor
					RUNTIME_OUTPUT_DIRECTORY_DEBUG		${CMAKE_SOURCE_DIR}/bin/Debug/SoloEditor
					RUNTIME_OUTPUT_DIRECTORY_RELEASE	${CMAKE_SOURCE_DIR}/bin/Release/SoloEditor
					RUNTIME_OUTPUT_DIRECTORY_DIST		${CMAKE_SOURCE_DIR}/bin/Dist/SoloEditor
					VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

check_ipo_supported(RESULT LTO_SUPPORT OUTPUT output)
if(LTO_SUPPORT)
	message(STATUS "-- Enabled Link-Time Optimization (LTO)!")
	set_target_properties(SoloEditor PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
	message(WARNING "IPO is not supported: ${output}")
endif()

add_dependencies(SoloEditor SoloEngine)


target_include_directories(SoloEditor PRIVATE
							${CMAKE_SOURCE_DIR}/SoloEngine/src
							${CMAKE_SOURCE_DIR}/SoloEngine/vendor
							${CMAKE_SOURCE_DIR}/SoloEngine/vendor/entt-src/single_include/entt
							${CMAKE_SOURCE_DIR}/SoloEngine/vendor/filewatch-src
							${CMAKE_SOURCE_DIR}/SoloEngine/vendor/glm-src
							${CMAKE_SOURCE_DIR}/SoloEngine/vendor/imgui-src
							${CMAKE_SOURCE_DIR}/SoloEngine/vendor/imguizmo-src
							${CMAKE_SOURCE_DIR}/SoloEngine/vendor/sol2-src/include
							${CMAKE_SOURCE_DIR}/SoloEngine/vendor/spdlog-src/include
)

target_compile_definitions(SoloEditor PRIVATE
							$<$<CONFIG:Debug>:SOLO_DEBUG>
							$<$<CONFIG:Release>:SOLO_RELEASE>
							$<$<CONFIG:Release>:TRACY_ENABLE>
							$<$<CONFIG:Release>:TRACY_ON_DEMAND>
							$<$<CONFIG:Dist>:SOLO_DIST>
)

if(MSVC)
	target_link_options(SoloEditor PRIVATE
								$<$<CONFIG:Release>:/INCREMENTAL:NO>
								$<$<CONFIG:Release>:/DEBUG>
								$<$<CONFIG:Release>:/OPT:REF> # Remove unreferenced functions and data
								$<$<CONFIG:Release>:/OPT:ICF> # Identical COMDAT folding
	)
	endif()

target_link_libraries(SoloEditor
					SoloEngine
)



if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	#target_compile_options(SoloEditor PRIVATE /W4)
else()
	target_compile_options(SoloEditor PRIVATE -Wall -Wextra -Wundef -Wno-cast-function-type -pedantic -Wno-long-long -Wshadow -Werror -Wno-error=deprecated-declarations)
endif()