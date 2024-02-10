

target_sources(SoloEngine PUBLIC 
"App.h"
"EntryPoint.h"
"System.h"
"Logger.h"
"App.cpp"
"LayerStack.h"
LayerStack.cpp
AppLayer.h
AppLayer.cpp
TimeStep.h

PRIVATE
)


 target_include_directories(SoloEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})