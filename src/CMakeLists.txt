cmake_minimum_required(VERSION 3.5)

add_executable(search_engine
        main.cpp
        converterJSON.cpp ${INCLUDE}/converterJSON.h
        InvertedIndex.cpp ${INCLUDE}/InvertedIndex.h
        SearchServer.cpp ${INCLUDE}/SearchServer.h
)

target_include_directories(search_engine PUBLIC ${INCLUDE})

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

target_link_libraries(search_engine PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(search_engine)

#nlohmann_json
set(JSON_BuildTests OFF CACHE INTERNAL "")
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)