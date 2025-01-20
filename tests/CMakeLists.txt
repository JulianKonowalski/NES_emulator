set(gtest_force_shared_crt on)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        main
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

set(
	MOS6502_TEST_SRC
	${CMAKE_CURRENT_LIST_DIR}/MOS6502/MOS6502_tests.cpp
	${CMAKE_CURRENT_LIST_DIR}/MOS6502/AddressingMode_tests.cpp
)

add_executable(
	NES_tests
	${MOS6502_TEST_SRC}
) 

target_link_libraries(
	NES_tests
	PRIVATE
	GTest::GTest
	MOS6502
)

add_test(MOS6502_gtests MOS6502_tests)