
set(target SoloCore)

set(sources
    Engine.cpp
    EventRouter.cpp
)

set(headers
    Engine.h
    EventRouter.h
    Logger.h
    ThreadSafe.h
    EventBase.h
)

add_library(${target}
    ${sources}
    ${headers}
)