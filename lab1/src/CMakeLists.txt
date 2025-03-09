cmake_minimum_required(VERSION 3.29)
project(cc_lab1)

#add_compile_options(-Wno-unimplemented)

set(CMAKE_CXX_STANDARD 23)

find_package(Boost REQUIRED)

include_directories(include)

add_library(myLib STATIC
        AutomationFactory.hpp
        AutomationFactory.cpp
        NFAFactory.hpp
        NFAFactory.cpp
        DFAFactory.hpp
        DFAFactory.cpp
        NFABuilder.hpp
        NFABuilder.cpp
        DFABuilder.hpp
        DFABuilder.cpp
        AutomationVizualizator.hpp
        AutomationVizualizator.cpp
        NFA.hpp
        NFA.cpp
        DFA.hpp
        DFA.cpp
        MinimizedFA.hpp
        MinimizedFA.cpp
        MyStack.hpp
        DFAMinimizator.hpp
        DFAMinimizator.cpp
        include/ObservableProperty.hpp
        include/ObservableProperty.cpp
        include/Utils.h
        include/Utils.cpp
        include/ObservableProperty.hpp
)

add_executable(lab1
        RegexParser.cpp
)

target_include_directories(myLib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
)

target_include_directories(lab1 PRIVATE
        ${Boost_INCLUDE_DIRS})

target_link_libraries(myLib PRIVATE ${Boost_LIBRARIES})

target_link_libraries(lab1 PRIVATE myLib)
