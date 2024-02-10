target_sources(SoloEngine PUBLIC 
"AudioSystem.h"
"AudioSystem.cpp"
PRIVATE
)


 target_include_directories(SoloEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})