cmake_minimum_required(VERSION 3.22)
project(EnvHelper)

set(CMAKE_CXX_STANDARD 20)

# Header-only library
add_library(EnvHelper INTERFACE)
target_include_directories(EnvHelper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Only add test executable in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(EnvHelperTest main.cpp)
    target_link_libraries(EnvHelperTest PRIVATE EnvHelper)
    target_include_directories(EnvHelperTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()
