cmake_minimum_required(VERSION 3.5)

set(SrcFiles
        ${SRC}/converterJSON.cpp
        ${SRC}/InvertedIndex.cpp
        ${SRC}/SearchServer.cpp
)

set(IncludeFiles
        ${INCLUDE}/converterJSON.h
        ${INCLUDE}/InvertedIndex.h
        ${INCLUDE}/SearchServer.h
)

add_executable(test_engine
        test.cpp
        ${SrcFiles}
        ${IncludeFiles}
)

target_include_directories(test_engine PUBLIC ${INCLUDE})

#gtest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_MakeAvailable(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

target_link_libraries(test_engine PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(test_engine)

#nlohmann_json
set(JSON_BuildTests OFF CACHE INTERNAL "")
target_link_libraries(test_engine PRIVATE nlohmann_json::nlohmann_json)