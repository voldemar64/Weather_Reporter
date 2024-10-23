if(WIN32) # Install dlls in the same directory as the executable on Windows
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

cmake_minimum_required(VERSION 3.20)

project(
        labwork7
        VERSION 1.0
        DESCRIPTION "C++ Year 2024"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(bin)
add_subdirectory(libs)
