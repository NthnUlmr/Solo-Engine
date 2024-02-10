

target_sources(SoloEngine PUBLIC 
"AppEvent.h"
"Event.h"
"GamepadEvent.h"
"KeyEvent.h"
"MouseEvent.h"


PRIVATE
)


 target_include_directories(SoloEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})