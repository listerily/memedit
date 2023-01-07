cmake_minimum_required(VERSION 3.25)
project(memedit)

set(CMAKE_CXX_STANDARD 17)

add_executable(memedit src/main.cpp src/util.h src/util.cpp src/screen.cpp src/screen.h src/color.cpp src/color.h src/proc.cpp src/proc.h src/mem.cpp src/mem.h src/routines.cpp src/routines.h)
target_link_libraries(memedit PUBLIC ncurses)