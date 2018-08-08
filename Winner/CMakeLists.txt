cmake_minimum_required(VERSION 3.10)
project(Winner)

set(CMAKE_CXX_STANDARD 11)

add_executable(Winner main.cpp Judge.h Judge.cpp Ruler.h Ruler.cpp WinnerPokerCommon.h WinnerPokerCommon.cpp)