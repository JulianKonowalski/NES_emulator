set(
    IO_SOURCES
    "Window.cpp"
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