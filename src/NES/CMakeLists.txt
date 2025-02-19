add_subdirectory(Buses)
add_subdirectory(MOS6502)
add_subdirectory(Cartridge)
add_subdirectory(PPU2C02)

set(
	NES_SOURCES
	${CMAKE_CURRENT_LIST_DIR}/NES.cpp
)

add_library(
	NES
	${NES_SOURCES}
)

target_link_libraries(
	NES
	PRIVATE
	MOS6502
	PPU2C02
	BUSES
	CARTRIDGE
)