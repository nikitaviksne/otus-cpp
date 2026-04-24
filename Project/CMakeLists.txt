cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(SignalSlotLibrary VERSION ${PROJECT_VESRION})

option(WITH_GOOGLE_TEST "Whether to build Google test" ON) 

add_library(signal_slot INTERFACE)
target_include_directories(signal_slot INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/signal-slot)

set_target_properties(signal_slot PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)


#Google tests
if(WITH_GOOGLE_TEST)
    #message(STATUS, "before enable")
    enable_testing()
    #message(STATUS, "before include")
    include(GoogleTest)
    #message(STATUS, "before subdirectory")
    add_subdirectory(tests)
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest)

# Google Benchmarks
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE) # отключение тестов самого бенчмарка
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
include(FetchContent)
FetchContent_Declare(
  google_benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.5
)
FetchContent_MakeAvailable(google_benchmark)

add_executable(run_benchmarks tests/benchmarks.cpp)
target_link_libraries(run_benchmarks PRIVATE signal_slot benchmark::benchmark)


install(TARGETS signal_slot RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)



