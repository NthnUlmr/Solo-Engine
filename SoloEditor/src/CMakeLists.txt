set(target SoloEditor)

set(sources
    Main.cpp
)

set(headers
    
)

add_executable(${target}
    ${sources}
    ${headers}
)

target_link_libraries(${target} PUBLIC
    SoloCore
)

target_include_directories(${target} PUBLIC
    ${CMAKE_SOURCE_DIR}/SoloEngine
)