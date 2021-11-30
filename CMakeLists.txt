cmake_minimum_required(VERSION 3.16)
project(CXXAutomata)

set(CMAKE_CXX_STANDARD 14)

# Setup testing
enable_testing()
include(GoogleTest)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})


include_directories(Src/Automaton)
include_directories(Src/Common)
include_directories(Src/Exceptions)
include_directories(Src/FA)

add_library(CXXAutomata SHARED
        Src/Automaton/Automaton.cpp
        Src/Exceptions/Exceptions.cpp
        Src/FA/DFA.cpp
        Src/FA/FA.cpp)




add_executable(CXXAutomataTest  Test/main.cpp
                                Test/testDFA.cpp)
target_link_libraries(CXXAutomataTest CXXAutomata gtest pthread)

gtest_discover_tests(CXXAutomataTest Test/main.cpp
                                Test/testDFA.cpp
)
