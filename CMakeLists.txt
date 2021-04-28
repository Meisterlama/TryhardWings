cmake_minimum_required(VERSION 3.17)
project(TryHardWings LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} --cpuprofiler -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY --embed-file ../assets")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
endif ()

add_subdirectory(raylib)

file(GLOB_RECURSE TRY_HARD_WINGS_SRC
        src/*.cpp)


add_executable(TryHardWings ${TRY_HARD_WINGS_SRC})
target_include_directories(TryHardWings PUBLIC include)
target_link_libraries(TryHardWings raylib)
target_compile_options(TryHardWings PRIVATE -fpermissive)