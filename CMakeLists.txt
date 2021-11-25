cmake_minimum_required(VERSION 3.21)
project(CXXAutomata)

set(CMAKE_CXX_STANDARD 14)

include_directories(Src/Automaton)
include_directories(Src/Common)
include_directories(Src/Exceptions)
include_directories(Src/FA)

add_library(CXXAutomata SHARED
        Src/Automaton/Automaton.cpp
        Src/Exceptions/Exceptions.cpp
        Src/FA/DFA.cpp
        Src/FA/FA.cpp)
