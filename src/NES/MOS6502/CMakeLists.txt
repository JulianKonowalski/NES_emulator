set(
	MOS6502_SOURCES
	${CMAKE_CURRENT_LIST_DIR}/MOS6502.cpp
	${CMAKE_CURRENT_LIST_DIR}/AddressingMode.cpp
	${CMAKE_CURRENT_LIST_DIR}/Instruction.cpp
	${CMAKE_CURRENT_LIST_DIR}/Operation.cpp
)

add_library(
	MOS6502
	${MOS6502_SOURCES}
)

target_link_libraries(
	MOS6502
	BUSES
)