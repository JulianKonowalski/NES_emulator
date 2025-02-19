set(
    IO_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Screen.cpp
)

add_library(
    IO
    ${IO_SOURCES}
)

target_link_libraries(
    IO
    PRIVATE
    raylib_static
)