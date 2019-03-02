cmake_minimum_required(VERSION 3.8.2)

project(json)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_SOURCE_DIR}/include/config.h"
)

option (JSON_USE_PARSER_SPIRIT  "use parser spirit" ON)

if (JSON_USE_PARSER_SPIRIT)
    find_package(Boost REQUIRED)
    if (NOT Boost_FOUND)
        message("Boost Not Foound")
        return()
    endif()

    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif() 

include_directories(include)

# test parser
set(TGT_TEST_PARSER test_parser)
add_executable(${TGT_TEST_PARSER} test/test_parser.cpp)

set(TGT_TEST_JSON test_json)
add_executable(${TGT_TEST_JSON} test/test_json.cpp)

set(TGT_TEST_VALUE test_value)
add_executable(${TGT_TEST_VALUE} test/test_value.cpp)

FILE(GLOB src_files include/*)
INSTALL(FILES ${src_files} DESTINATION include/json)


