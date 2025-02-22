cmake_minimum_required(VERSION 3.29)
project(cc_lab1)

set(CMAKE_CXX_STANDARD 23)

#set(BOOST_ROOT /Users/matvey_agarkov/Desktop/third_party/boost)

find_package(Boost REQUIRED)

include_directories(include)

add_executable(lab1
        RegexParser.cpp
        include/Utils.h
        include/Utils.cpp
        #include/IAutomation.hpp
        #include/IAutomationBuilder.hpp
        #include/IAutomationFactory.hpp
        #include/IAutomationVizu.hpp
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
        MyStack.hpp
        NFA.hpp)

target_include_directories(lab1 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(lab1 PRIVATE ${Boost_LIBRARIES})