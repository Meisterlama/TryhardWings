cmake_minimum_required(VERSION 3.17)
project(TryHardWings LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(raylib)

file(GLOB_RECURSE TRY_HARD_WINGS_SRC
        src/*.cpp)

add_executable(TryHardWings ${TRY_HARD_WINGS_SRC})
target_include_directories(TryHardWings PUBLIC include)
target_link_libraries(TryHardWings raylib)