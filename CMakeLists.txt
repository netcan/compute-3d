# author by netcan
# @shanghai 2024/09/23 23:29
cmake_minimum_required(VERSION 3.20)
project(compute-3d LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_C_FLAGS "-std=c99 -Wall")
set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -g -ggdb")
set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O3")
set(CMAKE_CXX_FLAGS "-Wall -Wno-sign-compare -Wno-return-type -Wno-unused-result -fno-rtti -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g -ggdb -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3")

include_directories(
    include
    third_party)

add_executable(compute-3d
    src/main.cpp
    src/CustomRendering.cpp
    src/Model.cpp
    src/StbImpl.cpp
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_demo.cpp
    third_party/imgui/backends/imgui_impl_sdl2.cpp
    third_party/imgui/backends/imgui_impl_sdlrenderer2.cpp
    )

target_include_directories(compute-3d
    PRIVATE ${CMAKE_SOURCE_DIR}/third_party/imgui/)
target_link_libraries(compute-3d SDL2)

if (APPLE)
    target_link_directories(compute-3d PRIVATE /opt/homebrew/opt/llvm/lib/c++)
endif()

add_subdirectory(third_party)
