cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(matrix VERSION ${PROJECT_VESRION})

option(WITH_GOOGLE_TEST "Whether to build Google test" ON) 

add_executable(matrix main.cpp)

target_include_directories(matrix
    PRIVATE "${CMAKE_BINARY_DIR}"
)

set_target_properties(matrix PROPERTIES
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
install(TARGETS matrix RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)



