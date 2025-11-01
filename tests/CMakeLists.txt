include(GoogleTest)
set(GooTestArgs ) # Goo -- Google GooTestArgs empty
include_directories(../)
add_executable(GooTest test_version.cpp)
gtest_add_tests(GooTest "${GooTestArgs}" AUTO)
