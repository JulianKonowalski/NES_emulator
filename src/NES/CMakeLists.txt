add_subdirectory(Buses)
add_subdirectory(MOS6502)
add_subdirectory(Cartridge)
add_subdirectory(PPU2C02)
add_subdirectory(APU)

set(
	NES_SOURCES
	NES.cpp
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
	APU
	BUSES
	CARTRIDGE
)