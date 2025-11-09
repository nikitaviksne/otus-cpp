cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(filter_ip VERSION ${PROJECT_VESRION})

option(WITH_BOOST_TEST "Whether to build Boost test" OFF)
option(WITH_GOOGLE_TEST "Whether to build Google test" ON) 

configure_file(version.h.in version.h)

add_executable(ip_filter main.cpp)
add_library(version lib.cpp)
add_library(ip_algorithms process_ip.cpp)

set_target_properties(ip_filter version PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(ip_filter ip_algorithms PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(version
    PRIVATE "${CMAKE_BINARY_DIR}"
)
target_include_directories(ip_algorithms
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(ip_filter PRIVATE
    version
    ip_algorithms
)


#Google tests
if(WITH_GOOGLE_TEST)
    #message(STATUS, "before enable")
    enable_testing()
    #message(STATUS, "before include")
    include(GoogleTest)
    #message(STATUS, "before subdirectory")
    add_subdirectory(tests)
    #target_link_libraries(GooTest
#	helloworld
#	#tests
#    )
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest)
#add_executable(GooTest IMPORTED)
#target_link_libraries(test_version helloworld)

#target_link_libraries(GooTest gtest gtest_main ${PROJECT_NAME})


install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_BOOST_TEST)
    enable_testing()
    add_test(test_version test_version)
endif()
