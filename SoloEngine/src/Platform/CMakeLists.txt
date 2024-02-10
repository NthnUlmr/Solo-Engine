set(HEADERS
   ${HEADERS}
	${CMAKE_CURRENT_SOURCE_DIR}/"DetectPlatform.h"
	PARENT_SCOPE
)

add_subdirectory(WindowsVulkan)

target_include_directories(SoloEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})