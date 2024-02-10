include(CMakePrintHelpers)
include(CheckIPOSupported)

project(SoloEngine VERSION 0.0.1 LANGUAGES CXX)


add_subdirectory(src)
add_subdirectory(vendor)

find_package(Vulkan REQUIRED COMPONENTS glslc glslang SPIRV-Tools)


set_target_properties(SoloEngine PROPERTIES
					ARCHIVE_OUTPUT_DIRECTORY_DEBUG 		${CMAKE_SOURCE_DIR}/bin/Debug/SoloEngine
					ARCHIVE_OUTPUT_DIRECTORY_RELEASE 	${CMAKE_SOURCE_DIR}/bin/Release/SoloEngine
					ARCHIVE_OUTPUT_DIRECTORY_DIST 		${CMAKE_SOURCE_DIR}/bin/Dist/SoloEngine
					LIBRARY_OUTPUT_DIRECTORY_DEBUG 		${CMAKE_SOURCE_DIR}/bin/Debug/SoloEngine
					LIBRARY_OUTPUT_DIRECTORY_RELEASE 	${CMAKE_SOURCE_DIR}/bin/Release/SoloEngine
					LIBRARY_OUTPUT_DIRECTORY_DIST 		${CMAKE_SOURCE_DIR}/bin/Dist/SoloEngine
					RUNTIME_OUTPUT_DIRECTORY_DEBUG 		${CMAKE_SOURCE_DIR}/bin/Debug/SoloEngine
					RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${CMAKE_SOURCE_DIR}/bin/Release/SoloEngine
					RUNTIME_OUTPUT_DIRECTORY_DIST 		${CMAKE_SOURCE_DIR}/bin/Dist/SoloEngine
)

check_ipo_supported(RESULT LTO_SUPPORT OUTPUT output)
if(LTO_SUPPORT)
	message(STATUS "-- Enabled Link-Time Optimization (LTO)!")
	set_target_properties(SoloEngine PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
	message(WARNING "IPO is not supported: ${output}")
endif()


target_include_directories(SoloEngine PRIVATE src ${VULKAN_INCLUDE_DIRS})

target_compile_definitions(SoloEngine PRIVATE
							GLFW_EXPOSE_NATIVE_WIN32
							_CRT_SECURE_NO_WARNINGS
							#GLFW_INCLUDE_NONE
							YAML_CPP_STATIC_DEFINE
							$<$<CONFIG:Debug>:SOLO_DEBUG>
							$<$<CONFIG:Release>:SOLO_RELEASE>
							$<$<CONFIG:Dist>:SOLO_DIST>
)

target_link_libraries(SoloEngine  Vulkan::Vulkan opengl32 )



if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	#target_compile_options(SoloEngine PUBLIC /W3)
else()
	target_compile_options(SoloEngine PRIVATE -Wall -Wextra -Wundef -Wno-cast-function-type -pedantic -Wno-long-long -Wshadow -Werror -Wno-error=deprecated-declarations)
endif()