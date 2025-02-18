set(
    BUSES_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/CPUBus.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PPUBus.cpp
)

add_library(
    BUSES 
    ${BUSES_SOURCES}
)