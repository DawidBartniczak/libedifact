cmake_minimum_required(VERSION 3.27.0)
project(edi)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(sample)