cmake_minimum_required(VERSION 3.15)
project(alpha-ants)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address,undefined)
    add_compile_options(-fno-omit-frame-pointer -fsanitize=address,undefined)
    add_link_options   (-fno-omit-frame-pointer -fsanitize=address,undefined)
    add_definitions(-DDEBUG)
endif ()

add_executable(alpha-ants
        src/bot.cpp
        src/my_bot.cpp
        src/state.cpp)