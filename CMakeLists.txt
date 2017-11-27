cmake_minimum_required(VERSION 3.5)
project(media_manage_system)

include(CTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wno-unused-macros")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --[ Includes
set(TS_THIRD_PARTY_GTEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party/gmock)

include_directories(AFTER ${TS_THIRD_PARTY_GTEST_INCLUDE_DIR})
include_directories(./third_party/corgi/output/include)
include_directories(./third_party/brpc/output/include)
include_directories(./third_party/concurrentqueue)
include_directories(BEFORE src)

# --[ using cmake scripts and modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

include(ExternalProject)

include(cmake/Utils.cmake)
include(cmake/Targets.cmake)
include(cmake/Misc.cmake)
include(cmake/CompilerInfo.cmake)
include(cmake/Summary.cmake)
include(cmake/ConfigGen.cmake)

include(cmake/Dependencies.cmake)
# ------

link_directories(./third_party/corgi/output/lib)
link_directories(./third_party/brpc/output/lib)

add_subdirectory(src)
add_subdirectory(test)
