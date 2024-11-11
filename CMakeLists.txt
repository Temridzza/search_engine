cmake_minimum_required(VERSION 3.5)

project(search_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE ${CMAKE_SOURCE_DIR}/include)
find_package(Python3 COMPONENTS Interpreter)

add_subdirectory(src)

#nlohmann_json
add_subdirectory(nlohmann_json)

