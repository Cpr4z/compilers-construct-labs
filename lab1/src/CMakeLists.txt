cmake_minimum_required(VERSION 3.29)
project(cc_lab1)

set(CMAKE_CXX_STANDARD 23)

#set(BOOST_ROOT /Users/matvey_agarkov/Desktop/third_party/boost)

find_package(Boost REQUIRED)

include_directories(include)

add_executable(lab1
        RegexParser.cpp
        include/Utils.h)

target_include_directories(lab1 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(lab1 PRIVATE ${Boost_LIBRARIES})