cmake_minimum_required(VERSION 3.8.2)

project(json)

set(TARGET_LIB json)

add_compile_options(-std=c++14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Boost REQUIRED)
if (NOT Boost_FOUND)
    message("Boost Not Foound")
endif()

include_directories(${Boost_INCLUDE_DIRS})

 
file (GLOB SRC_FILE src/*.h src/*.cpp src/*.hpp)

message(${SRC_FILE})

add_executable(${TARGET_LIB} ${SRC_FILE})

#add_library(${TARGET_LIB} STATIC ${SRC_FILE})
#set_target_properties(${TARGET_LIB} PROPERTIES LINKER_LANGUAGE CXX) 


#add_executable(main main.cpp)
#target_link_libraries(${TARGET_LIB})


