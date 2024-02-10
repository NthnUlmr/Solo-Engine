target_sources(SoloEngine PUBLIC 
"Scene.h"
"Camera.h"
"Camera.cpp"
PRIVATE
"Scene.cpp"
)


 target_include_directories(SoloEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})