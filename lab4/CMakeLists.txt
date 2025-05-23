cmake_minimum_required(VERSION 3.29)
project(lab4)

find_package( Boost REQUIRED COMPONENTS filesystem )

set (
        HEADERS
        graphviz.hpp
        grammar.hpp
        node.hpp
        parser.hpp
        tokenizator.hpp
        utils.hpp
)

set (
        SOURCES
        graphviz.cpp
        parser.cpp
        tokenizator.cpp
        node.cpp
        node.cpp
)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} main.cpp)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(${PROJECT_NAME} INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Werror;-Wall;-Wextra;-Wpedantic-Wcast-align;-Wcast-qual;-Wconversion;-Wctor-dtor-privacy;-Wenum-compare;-Wextra-semi;-Wfloat-equal;-Wnon-virtual-dtor;-Wold-style-cast;-Woverloaded-virtual;-Wredundant-decls;-Wshadow;-Wsign-conversion;-Wsign-promo;-Wzero-as-null-pointer-constant>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W4>>"
)


target_link_libraries( ${PROJECT_NAME} PRIVATE Boost::filesystem)