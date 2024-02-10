target_sources(SoloEngine PUBLIC 
"PhysicsSystem.h"
PRIVATE
"PhysicsSystem.cpp"
)


 target_include_directories(SoloEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})