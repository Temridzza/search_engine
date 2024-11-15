cmake_minimum_required(VERSION 3.5)

set(IncludeFiles
        ${INCLUDE}/converterJSON.h
        ${INCLUDE}/InvertedIndex.h
        ${INCLUDE}/SearchServer.h
)

set(SrcFiles
        converterJSON.cpp
        InvertedIndex.cpp
        SearchServer.cpp
)

add_executable(search_engine
        main.cpp
        ${IncludeFiles}
        ${SrcFiles}
)

target_include_directories(search_engine PUBLIC ${INCLUDE})


#nlohmann_json
set(JSON_BuildTests OFF CACHE INTERNAL "")
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)