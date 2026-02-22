cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(sfinae VERSION ${PROJECT_VESRION})



add_executable(sfinae sfinae.cpp)


set_target_properties(sfinae PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)




