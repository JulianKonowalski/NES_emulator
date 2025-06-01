set(
    APU_SOURCES 
    APU.cpp
    Oscillator.cpp
    DMC.cpp
)

add_library(
    APU
    ${APU_SOURCES}
)

target_link_libraries(
    APU
    PRIVATE
    BUSES
    raylib_static
)