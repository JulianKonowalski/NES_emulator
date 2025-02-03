add_subdirectory ("MOS6502")
add_subdirectory ("NES")

target_include_directories(MOS6502 PUBLIC include)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    NES
)