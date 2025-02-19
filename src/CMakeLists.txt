add_subdirectory(NES)
add_subdirectory(IO)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    NES
    IO
)