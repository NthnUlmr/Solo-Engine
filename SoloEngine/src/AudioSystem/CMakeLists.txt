target_sources(SoloEngine PUBLIC 
"AudioSystem.h"
"AudioSystem.cpp"
"AudioEngine.h"
"AudioEngine.cpp"
PRIVATE
)


target_link_libraries(SoloEngine
"C:\\Program Files (x86)\\OpenAL 1.1 SDK\\libs\\Win64\\OpenAL32.lib"
)

 target_include_directories(SoloEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR}
 "C:\\Program Files (x86)\\OpenAL 1.1 SDK\\include"
 )

