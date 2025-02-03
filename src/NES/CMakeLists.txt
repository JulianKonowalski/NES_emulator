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
)