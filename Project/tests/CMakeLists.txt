cmake_minimum_required(VERSION 3.12)
enable_testing()

#configure_file(../version.h.in version.h)
add_executable(GooTest test.cpp)
target_include_directories(GooTest PUBLIC GoogleTest)
target_include_directories(GooTest PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
					"${CMAKE_CURRENT_SOURCE_DIR}/include/signal-slot/"
)

target_link_libraries(GooTest PRIVATE 
	GTest::gtest_main
	GTest::gmock
	signal_slot
)
#gtest_add_tests(GooTest "${GooTestArgs}" AUTO)
gtest_discover_tests(GooTest)
