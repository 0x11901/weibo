cmake_minimum_required(VERSION 3.10)
project(Winner)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_COMPILER "/usr/local/bin/gcc-4.9")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-4.9")

add_executable(Winner main.cpp Judge.cpp Judge.h Ruler.cpp Ruler.h WinnerPokerCommon.cpp WinnerPokerCommon.h)