add_subdirectory ("MOS6502")
add_subdirectory ("NES")

add_executable(${PROJECT_NAME} main.cpp "Buses/CPUBus.cpp" "Buses/PPUBus.cpp")

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    NES
)