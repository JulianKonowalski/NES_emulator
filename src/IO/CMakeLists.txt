set(
    IO_SOURCES
    Window.cpp
)   

add_library(
    IO
    ${IO_SOURCES}
)

target_link_libraries(
    IO
    PRIVATE
    raylib_static
    jsoncpp_lib_static
)

target_compile_definitions(
  IO
  PRIVATE
  CONFIG_PATH="${CONFIG_FILE_PATH}"
)