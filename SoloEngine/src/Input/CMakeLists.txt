

target_sources(SoloEngine PUBLIC 
"GamepadCodes.h"
InputManager.cpp
InputManager.h
KeyCodes.h
MouseCodes.h

PRIVATE
)


 target_include_directories(SoloEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})