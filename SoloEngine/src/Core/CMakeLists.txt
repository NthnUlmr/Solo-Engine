
set(target SoloCore)

set(sources
    Engine.cpp
)

set(headers
    Engine.h
)

add_library(${target}
    ${sources}
    ${headers}
)