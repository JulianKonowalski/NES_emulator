#ifndef GTEST
	#define GTEST
	#include <gtest/gtest.h> //multiple includes of this header cause compilation errors
#endif // !GTEST

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}