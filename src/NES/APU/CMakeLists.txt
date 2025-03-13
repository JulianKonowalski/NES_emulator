set(
    APU_SOURCES 
    APU.cpp
    Oscillator.cpp
)

add_library(
    APU
    ${APU_SOURCES}
)

target_link_libraries(
    APU
    PRIVATE
    raylib_static
)