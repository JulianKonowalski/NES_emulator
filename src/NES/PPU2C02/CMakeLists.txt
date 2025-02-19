set(
    PPU_SOURCES 
    PPU2C02.cpp
)

add_library(
    PPU2C02 
    ${PPU_SOURCES}
)

target_link_libraries(
    PPU2C02
    PRIVATE
    IO
)