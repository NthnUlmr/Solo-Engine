

target_sources(SoloEngine PUBLIC 
"Octree.h"
"Octree.cpp"
PRIVATE
)


 target_include_directories(SoloEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})